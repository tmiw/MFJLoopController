#include <LittleFS.h>
#include <ESP8266mDNS.h>
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
  
  server_.on(F("/manual_tune"), [&]() { handleManualTune_(); });
  server_.on(F("/autotune"), [&]() { handleAutoTune_(); });
  server_.on(F("/status"), [&]() { handleStatus_(); });
  server_.onNotFound([&]() { handleNotFound_(); });
  server_.begin();
}

void WebServerController::process()
{
  server_.handleClient();
  MDNS.update();
}

void WebServerController::replyBadRequest_(String msg)
{
  server_.send(400, FPSTR(TEXT_PLAIN), msg + F("\r\n"));
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

void WebServerController::handleManualTune_()
{
  if (!server_.hasArg(F("enable")))
  {
    replyBadRequest_(F("Required: enable. + direction, speed if enable == true"));
    return;
  }
  
  String enable = server_.arg(F("enable"));
  
  if (enable == F("true"))
  {
    if (!server_.hasArg(F("direction")) || !server_.hasArg(F("speed")))
    {
      replyBadRequest_(F("Required: enable. + direction, speed if enable == true"));
      return;
    }

    String direction = server_.arg(F("direction"));
    String speed = server_.arg(F("speed"));

    CapacitorController::Speed capacitorSpeed = CapacitorController::IDLE;
    CapacitorController::Direction capacitorDirection = CapacitorController::NONE;
    
    if (speed == F("slow"))
    {
      capacitorSpeed = CapacitorController::SLOW;
    }
    else if (speed == F("fast"))
    {
      capacitorSpeed = CapacitorController::FAST;
    }
    else
    {
      replyBadRequest_(F("Invalid: speed (slow, fast)."));
      return;
    }
    
    if (direction == F("down"))
    {
      capacitorDirection = CapacitorController::DOWN;
    }
    else if (direction == F("up"))
    {
      capacitorDirection = CapacitorController::UP;
    }
    else
    {
      replyBadRequest_(F("Invalid: direction (down, up)."));
      return;
    }
    
    pCapacitorController_->setDirection(capacitorDirection);
    pCapacitorController_->setSpeed(capacitorSpeed);
  } 
  else 
  {
    pCapacitorController_->setDirection(CapacitorController::NONE);
    pCapacitorController_->setSpeed(CapacitorController::IDLE);
  }

  server_.send(200, FPSTR(TEXT_JSON), F("{ 'result': 'success' }"));
}

void WebServerController::handleAutoTune_()
{
  if (!server_.hasArg(F("enable")))
  {
    replyBadRequest_(F("Required: enable. + direction if enable == true"));
    return;
  }
  
  String enable = server_.arg(F("enable"));
  
  if (enable == F("true"))
  {
    if (!server_.hasArg(F("direction")))
    {
      replyBadRequest_(F("Required: enable. + direction if enable == true"));
      return;
    }

    String direction = server_.arg(F("direction"));

    AutoTuneController::Direction autoTuneDirection = AutoTuneController::NONE;   
    if (direction == F("down"))
    {
      autoTuneDirection = AutoTuneController::DOWN;
    }
    else if (direction == F("up"))
    {
      autoTuneDirection = AutoTuneController::UP;
    }
    else
    {
      replyBadRequest_(F("Invalid: direction (down, up)."));
      return;
    }
    
    pAutoTuneController_->beginTune(autoTuneDirection);
  } 
  else 
  {
    pAutoTuneController_->endTune();
  }

  server_.send(200, FPSTR(TEXT_JSON), F("{ 'result': 'success' }"));
}

void WebServerController::handleStatus_()
{
  server_.send(200, FPSTR(TEXT_JSON), 
              "{ \"result\": \"success\", "
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

//void WebServerController::handleAutotune_() { }
