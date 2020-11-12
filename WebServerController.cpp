#include <LittleFS.h>
#include <ESP8266mDNS.h>
#include <Arduino_JSON.h>
#include "WebServerController.h"
#include "CapacitorController.h"
#include "PowerMonitor.h"
#include "AutoTuneController.h"
#include "config.h"

static const char TEXT_PLAIN[] PROGMEM = "text/plain";
static const char TEXT_JSON[] PROGMEM = "text/json";

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
}

void WebServerController::process()
{
  server_.handleClient();
  MDNS.update();

  // Handle new connection attempts over WebSockets.
  if (socketServer_.poll())
  {
    auto client = socketServer_.accept();
    clientList_.push_back(client);
    sendStatusToClient_(client);
  }

  // Send current status to existing clients and handle client requests.
  std::vector<websockets::WebsocketsClient> activeList;
  for (auto& client : clientList_)
  {
    if (client.available())
    {
      client.onMessage([&](websockets::WebsocketsMessage msg) {
        handleClientRequest_(client, msg);
      });
      client.poll(); // Handle inbound messages
      
      sendStatusToClient_(client); // TODO: we don't need to send the current status every time through the loop.
      activeList.push_back(client);
    }
    else
    {
      client.close();
    }
  }

  clientList_ = activeList;
}

void WebServerController::handleNotFound_()
{
  // If we're able to find the file on the local FS, send it to the user.
  String path = ESP8266WebServer::urlDecode(server_.uri());
  if (path.endsWith(F("/")))
  {
    path += F("index.html");
  }

  if (LittleFS.exists(path))
  {
    File f = LittleFS.open(path, "r");
    server_.streamFile(f, mime::getContentType(path));
    f.close();
  }
  else
  {
    String message = F("File Not Found\n\n");
    message += F("URI: ");
    message += path;
    message += F("\nMethod: ");
    message += (server_.method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += server_.args();
    message += F("\n");
    for (uint8_t i = 0; i < server_.args(); i++) {
      message += " " + server_.argName(i) + ": " + server_.arg(i) + F("\n");
    }
    server_.send(404, F("text/plain"), message);
  }
}

void WebServerController::sendStatusToClient_(websockets::WebsocketsClient& client)
{
  // TODO
  client.send("{ \"command\": \"status\", \"success\": true, "
              "  \"fwd_power\": " + String(pPowerMonitor_->getForwardPower(), 1) + ", "
              "  \"rev_power\": " + String(pPowerMonitor_->getReversePower(), 1) + ", "
              "  \"fwd_power_adc\": " + String(pPowerMonitor_->getForwardPowerADC()) + ", "
              "  \"rev_power_adc\": " + String(pPowerMonitor_->getReversePowerADC()) + ", "   
              "  \"vswr\": " + String(pPowerMonitor_->getVSWR(), 2) + ", "   
              "  \"capacitor_speed\": " + String(pCapacitorController_->getSpeed()) + ", "
              "  \"capacitor_direction\": " + String(pCapacitorController_->getDirection()) + ","
              "  \"autotune_state\": " + String(pAutoTuneController_->getState()) + ","
              "  \"autotune_direction\": " + String(pAutoTuneController_->getDirection()) + "}");
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
  
