#include <LittleFS.h>
#include <ESP8266mDNS.h>
#include "WebServerController.h"
#include "CapacitorController.h"
#include "PowerMonitor.h"
#include "AutoTuneController.h"
#include "config.h"

static const char TEXT_PLAIN[] = "text/plain";
static const char TEXT_JSON[] = "text/json";

WebServerController::WebServerController(CapacitorController* pCapacitorController, PowerMonitor* pPowerMonitor, AutoTuneController* pAutoTuneController)
: server_(80)
, pCapacitorController_(pCapacitorController)
, pPowerMonitor_(pPowerMonitor)
, pAutoTuneController_(pAutoTuneController)
{
  // empty
}

WebServerController::~WebServerController()
{
  // empty
}

void WebServerController::setup()
{  
  if (MDNS.begin(MDNS_NAME)) {
    //Serial.println("MDNS responder started");
  }
  
  server_.onNotFound([&]() { handleNotFound_(); });
  server_.begin();

  socketServer_.listen(81);

  // Generate initial status output.
  currentStatus_ = generateStatusOutput_();
}

void WebServerController::process()
{
  server_.handleClient();
  MDNS.update();
  delay(0);
  
  // Handle new connection attempts over WebSockets.
  if (socketServer_.poll())
  {
    auto client = socketServer_.accept();
    clientList_.push_back(client);
    client.send(JSON.stringify(currentStatus_));
    delay(0);
  }

  // Send current status to existing clients and handle client requests.
  auto newStatus = generateStatusOutput_();
  auto iter = clientList_.begin();
  while (iter != clientList_.end())
  {
    if (iter->available())
    {
      iter->onMessage([&](websockets::WebsocketsMessage msg) {
        handleClientRequest_(*iter, msg);
      });
      iter->poll(); // Handle inbound messages

      sendStatusToClient_(*iter, newStatus);
      iter++;
    }
    else
    {
      iter->close();
      iter = clientList_.erase(iter);
    }
    delay(0);
  }

  currentStatus_ = newStatus;
}

void WebServerController::handleNotFound_()
{
  // If we're able to find the file on the local FS, send it to the user.
  String path = ESP8266WebServer::urlDecode(server_.uri());
  if (path.endsWith("/"))
  {
    path += "index.html";
  }
  
  if (LittleFS.exists(path))
  {
    File f = LittleFS.open(path, "r");

    // These two settings increase the reliability of the page load for some reason (!)
    // when sending "large" files (such as index.html). Ideally they wouldn't be needed,
    // so TBD for future.
    server_.client().setNoDelay(true);
    server_.client().setSync(true);

    int filesize = f.size();
    String WebString = "";
    WebString += "HTTP/1.1 200 OK\r\n";
    WebString += "Content-Type: text/html\r\n";
    WebString += "Content-Length: " + String(filesize) + "\r\n";
    WebString += "\r\n";
    server_.sendContent(WebString);
  
    //server_.streamFile(f, mime::getContentType(path));
    char buf[1024];
    int siz = f.size();
    while(siz > 0) {
      size_t len = std::min((int)(sizeof(buf) - 1), siz);
      f.read((uint8_t *)buf, len);
      server_.client().write((const char*)buf, len);
      siz -= len;

      delay(0);
    }
    f.close();
    server_.client().stop();
  }
  else
  {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += path;
    message += "\nMethod: ";
    message += (server_.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server_.args();
    message += "\n";
    for (uint8_t i = 0; i < server_.args(); i++) {
      message += " " + server_.argName(i) + ": " + server_.arg(i) + F("\n");
    }
    server_.send(404, "text/plain", message);
  }
}

JSONVar WebServerController::generateStatusOutput_()
{
  JSONVar output;
  output["command"] = "status";
  output["success"] = true;
  output["fwd_power"] = (double)pPowerMonitor_->getForwardPower();
  output["rev_power"] = (double)pPowerMonitor_->getReversePower();
  output["fwd_power_adc"] = (int)pPowerMonitor_->getForwardPowerADC();
  output["rev_power_adc"] = (int)pPowerMonitor_->getReversePowerADC();
  output["vswr"] = (double)pPowerMonitor_->getVSWR();
  output["capacitor_speed"] = (int)pCapacitorController_->getSpeed();
  output["capacitor_direction"] = (int)pCapacitorController_->getDirection();
  output["autotune_state"] = (int)pAutoTuneController_->getState();
  output["autotune_direction"] = (int)pAutoTuneController_->getDirection();
  return output;
}

void WebServerController::sendStatusToClient_(websockets::WebsocketsClient& client, JSONVar newStatus)
{
  if ((double)newStatus["fwd_power"] != (double)currentStatus_["fwd_power"] ||
      (double)newStatus["rev_power"] != (double)currentStatus_["rev_power"] ||
      (double)newStatus["vswr"] != (double)currentStatus_["vswr"] ||
      (int)newStatus["capacitor_speed"] != (int)currentStatus_["capacitor_speed"] ||
      (int)newStatus["capacitor_direction"] != (int)currentStatus_["capacitor_direction"] ||
      (int)newStatus["autotune_state"] != (int)currentStatus_["autotune_state"] ||
      (int)newStatus["autotune_direction"] != (int)currentStatus_["autotune_direction"])
  {
    client.send(JSON.stringify(newStatus));
  }
}

void WebServerController::handleClientRequest_(websockets::WebsocketsClient& client, websockets::WebsocketsMessage msg)
{
  auto input = JSON.parse(msg.data());
  if (JSON.typeof(input) == "undefined")
  {
    // Failed parse.
    JSONVar output;
    output["command"] = "unknown";
    output["success"] = false;
    output["reason"] = "Invalid JSON.";
    client.send(JSON.stringify(output));
    return;
  }

  if (!input.hasOwnProperty("command"))
  {
    // Missing command.
    JSONVar output;
    output["command"] = "unknown";
    output["success"] = false;
    output["reason"] = "Missing command.";
    client.send(JSON.stringify(output));
    return;
  }

  String command = String((const char*)input["command"]);
  if (command == "manual_tune" || command == "auto_tune")
  {
    AutoTuneController::Direction autoTuneDirection = AutoTuneController::NONE;
    CapacitorController::Speed capacitorSpeed = CapacitorController::IDLE;
    CapacitorController::Direction capacitorDirection = CapacitorController::NONE;

    if (!input.hasOwnProperty("enable"))
    {
      // Missing enable argument.
      JSONVar output;
      output["command"] = command;
      output["success"] = false;
      output["reason"] = "Required: enable.";
      client.send(JSON.stringify(output));
      return;
    }

    bool enable = input["enable"];

    if (enable)
    {
      if (!input.hasOwnProperty("direction"))
      {
        // Missing direction argument.
        JSONVar output;
        output["command"] = command;
        output["success"] = false;
        output["reason"] = "Required: direction.";
        client.send(JSON.stringify(output));
        return;
      }

      String direction = String((const char*)input["direction"]);
      if (command == "manual_tune")
      {
        if (direction == "down")
        {
          capacitorDirection = CapacitorController::DOWN;
        }
        else if (direction == "up")
        {
          capacitorDirection = CapacitorController::UP;
        }
        else
        {
          // Invalid direction argument.
          JSONVar output;
          output["command"] = command;
          output["success"] = false;
          output["reason"] = "Invalid direction.";
          client.send(JSON.stringify(output));
          return;
        }

        // Set manual tuning speed
        if (!input.hasOwnProperty("speed"))
        {
          // Missing speed argument.
          JSONVar output;
          output["command"] = command;
          output["success"] = false;
          output["reason"] = "Required: speed.";
          client.send(JSON.stringify(output));
          return;
        }

        auto speed = String((const char*)input["speed"]);
        if (speed == "slow")
        {
          capacitorSpeed = CapacitorController::SLOW;
        }
        else if (speed == "fast")
        {
          capacitorSpeed = CapacitorController::FAST;
        }
        else
        {
          // Invalid speed argument.
          JSONVar output;
          output["command"] = command;
          output["success"] = false;
          output["reason"] = "Invalid speed.";
          client.send(JSON.stringify(output));
          return;
        }
      }
      else
      {
        if (direction == "down")
        {
          autoTuneDirection = AutoTuneController::DOWN;
        }
        else if (direction == "up")
        {
          autoTuneDirection = AutoTuneController::UP;
        }
        else
        {
          // Invalid direction argument.
          JSONVar output;
          output["command"] = command;
          output["success"] = false;
          output["reason"] = "Invalid direction.";
          client.send(JSON.stringify(output));
          return;
        }
      }
    }

    // Commit new state.
    if (command == "auto_tune")
    {
      if (autoTuneDirection != AutoTuneController::NONE)
      {
        pAutoTuneController_->beginTune(autoTuneDirection);
      } 
      else 
      {
        pAutoTuneController_->endTune();
      }
    }
    else
    {
      pCapacitorController_->setDirection(capacitorDirection);
      pCapacitorController_->setSpeed(capacitorSpeed);
    }
        
    // Return success response.
    JSONVar output;
    output["command"] = command;
    output["success"] = true;
    output["at_dir"] = (long)autoTuneDirection;
    output["cap_speed"] = (long)capacitorSpeed;
    output["cap_dir"] = (long)capacitorDirection;
    output["enable"] = enable;
    client.send(JSON.stringify(output));
    return;
  }
  else if (command == "status")
  {
    // Send current status.
    client.send(JSON.stringify(currentStatus_));
    return;
  }
  else
  {
    // Invalid command.
    JSONVar output;
    output["command"] = command;
    output["success"] = false;
    output["reason"] = "Invalid command.";
    client.send(JSON.stringify(output));
    return;
  }
}
  
