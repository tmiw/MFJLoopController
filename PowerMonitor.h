#if !defined(POWER_MONITOR_H)
#define POWER_MONITOR_H

#include <Adafruit_ADS1015.h>
#include "ComponentController.h"

class PowerMonitor : ComponentController
{
public:
  PowerMonitor();
  virtual ~PowerMonitor();

  virtual void setup();
  virtual void process();

  double getForwardPower() const;
  double getReversePower() const;
  int16_t getForwardPowerADC() const;
  int16_t getReversePowerADC() const;
  double getVSWR() const;
  
private:
  Adafruit_ADS1115 ads1015_;
  double forwardPower_;
  int16_t forwardPowerAdc_;
  double revPower_;
  int16_t revPowerAdc_;
  double vswr_;
};

#endif // POWER_MONITOR_H
