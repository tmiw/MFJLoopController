#if !defined(WEB_SERVER_CONTROLLER_H)
#define WEB_SERVER_CONTROLLER_H

#include <ESP8266WebServer.h>
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
  
  void replyBadRequest_(String msg);
  void handleNotFound_();
  void handleManualTune_();
  void handleStatus_();
  void handleAutoTune_();
};

#endif // WEB_SERVER_CONTROLLER_H
