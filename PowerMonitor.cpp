#include <Wire.h>
#include "PowerMonitor.h"
#include "config.h"

PowerMonitor::PowerMonitor()
: forwardPower_(0.0)
, forwardPowerAdc_(0)
, revPower_(0.0)
, revPowerAdc_(0)
, vswr_(0.0)
{
  // empty
}

PowerMonitor::~PowerMonitor()
{
  // empty
}

/*ICACHE_RAM_ATTR void handleAdcAlertInterrupt()
{
  // Reverse tuning direction and go to next state
  set_tuning_direction(tuning_direction == TUNE_FWD ? TUNE_REV : TUNE_FWD);
  autotune_state = AUTOTUNE_SLOW;

  // Reset ADC chip to prevent re-raising interrupt
  ads1015.getLastConversionResults();
}*/

void PowerMonitor::setup()
{
  // Set up ADC object.
  ads1015_.begin();
  ads1015_.setGain(ADC_GAIN);
  //attachInterrupt(digitalPinToInterrupt(ADC_INTERRUPT_PIN), handleAdcAlertInterrupt, RISING);
}

void PowerMonitor::process()
{
  forwardPowerAdc_ = ads1015_.readADC_SingleEnded(ADC_FWD_CH);
  revPowerAdc_ = ads1015_.readADC_SingleEnded(ADC_REV_CH);

  // TBD: ADC->W conversion
  forwardPower_ = 0.0; //forwardPowerAdc_ * ((double)ADC_FWD_MAX_PO / (double)ADC_FWD_MAX_RAW);
  revPower_ = 0.0; //revPowerAdc_ * ((double)ADC_REV_MAX_PO / (double)ADC_REV_MAX_RAW);
  
  if (forwardPower_ > 0.0)
  {
    double coeff = sqrt(revPower_ / forwardPower_);
    vswr_ = (1 + coeff) / (1 - coeff);
  }
  else
  {
    vswr_ = 0.0;
  }
}

double PowerMonitor::getForwardPower() const
{
  return forwardPower_;
}

double PowerMonitor::getReversePower() const
{
  return revPower_;
}

int16_t PowerMonitor::getForwardPowerADC() const
{
  return forwardPowerAdc_;
}

int16_t PowerMonitor::getReversePowerADC() const
{
  return revPowerAdc_;
}

double PowerMonitor::getVSWR() const
{
  return vswr_;
}
