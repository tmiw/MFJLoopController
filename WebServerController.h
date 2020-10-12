#if !defined(WEB_SERVER_CONTROLLER_H)
#define WEB_SERVER_CONTROLLER_H

#include <ESP8266WebServer.h>
#include "ComponentController.h"

class CapacitorController;
class PowerMonitor;

class WebServerController : public ComponentController
{
public:
  WebServerController(CapacitorController* pCapacitorController, PowerMonitor* pPowerMonitor);
  virtual ~WebServerController();

  virtual void setup();
  virtual void process();
  
private:
  ESP8266WebServer server_;
  CapacitorController* pCapacitorController_;
  PowerMonitor* pPowerMonitor_;
  
  void replyBadRequest_(String msg);
  void handleNotFound_();
  void handleManualTune_();
  void handleStatus_();
  // void handleAutotune_();
};

#endif // WEB_SERVER_CONTROLLER_H
