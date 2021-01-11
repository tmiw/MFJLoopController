#if !defined(WEB_SERVER_CONTROLLER_H)
#define WEB_SERVER_CONTROLLER_H

#include <ESP8266WebServer.h>
#include <ArduinoWebsockets.h>
#include <Arduino_JSON.h>
#include <vector>
#include "ComponentController.h"

class CapacitorController;
class PowerMonitor;
class AutoTuneController;

class WebServerController : public ComponentController
{
public:
  WebServerController(CapacitorController* pCapacitorController, PowerMonitor* pPowerMonitor, AutoTuneController* pAutoTuneController);
  virtual ~WebServerController();

  virtual void setup();
  virtual void process();
  
private:
  ESP8266WebServer server_;
  CapacitorController* pCapacitorController_;
  PowerMonitor* pPowerMonitor_;
  AutoTuneController* pAutoTuneController_;
  websockets::WebsocketsServer socketServer_;
  std::vector<websockets::WebsocketsClient> clientList_;
  std::vector<unsigned int> lastHeardTime_;
  std::vector<bool> pongWaiting_;
  JSONVar currentStatus_;
  
  void handleNotFound_();
  JSONVar generateStatusOutput_();
  void sendStatusToClient_(websockets::WebsocketsClient& client, JSONVar newStatus);
  void handleClientRequest_(websockets::WebsocketsClient& client, websockets::WebsocketsMessage msg);
};

#endif // WEB_SERVER_CONTROLLER_H
