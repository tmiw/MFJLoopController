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

  // Truncate negative and other "low" ADC values to zero.
  /*if (revPowerAdc_ < 0)
  {
    revPowerAdc_ = 0;
  }
  if (forwardPowerAdc_ < 0)
  {
    forwardPowerAdc_ = 0;
  }*/
  
  forwardPower_ = powerFromAdc_(forwardPowerAdc_);
  revPower_ = powerFromAdc_(revPowerAdc_);

  // Truncate negative power values to zero.
  if (revPower_ < 0.0)
  {
    revPower_ = 0.0;
  }
  if (forwardPower_ < 0.0)
  {
    forwardPower_ = 0.0;
  }
  
  if (forwardPower_ > 0.0)
  {
    double coeff = sqrt(revPower_ / forwardPower_);
    vswr_ = (1 + coeff) / (1 - coeff);
  }
  else
  {
    vswr_ = 0.0;
    forwardPower_ = 0.0;
    revPower_ = 0.0;
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

double PowerMonitor::powerFromAdc_(int16_t adc)
{
  // Truncate "low" ADC values to zero due to issues with below curve fitting.
  if (adc < 500) return 0.0;

  adc *= 0.37;
  
  // Polynominal fitting @ degree 5 based on calculations at http://www.xuru.org/rt/PR.asp
  // and the following data:
  //
  //   3 0
  //   880 5
  //   1287 10
  //   1556 15
  //   1792 20
  //   2032 25
  //   2328 30
  //   2560 35
  //   2807 40
  //   3039 45
  //   3271 50
  //   3442 55
  //   3615 60
  //   3793 65
  //   3965 70
  //   4133 75
  //   4315 80
  //   4425 85
  //   4587 90
  //   4731 95
  //   4869 100
  //   eq: y = -1.02680951e-16 x^5 + 1.48581471e-12 x^4 - 7.729336489e-9 x^33 + 2.0030701e-5 x^2 - 7.576202086e-3 x + 6.452445678e-2
  //
  // Note: fwd and reverse use different equations due to slightly different readings (possibly
  // due to slight differences in toroid winding). Autotune algorithm should not be impacted
  // as it uses the lowest SWR regardless of what we calculate it to. You may also want to update 
  // the following equation if you're using a different implementation of the SWR bridge.
  return abs(
    -1.02680951e-16 * pow(adc, 5) +
    1.48581471e-12 * pow(adc, 4) - 
    7.729336489e-9 * pow(adc, 3) + 
    2.0030701e-5 * pow(adc, 2) - 
    7.576202086e-3 * adc + 
    6.452445678e-2);
}
