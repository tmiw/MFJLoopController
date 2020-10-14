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

  // Polynominal fitting @ degree 5 based on calculations at http://www.xuru.org/rt/PR.asp
  // and the following data:
  //
  // FWD (ADC, W):
  //   1738: 1
  //   2850: 5
  //   5104: 12
  //   7159: 25
  //   11468: 50
  //   17148: 100
  //   eq: y = 2.322509224e-18* x^5 - 9.260417149e-14*x^4 + 1.292824426e-9*x^3 - 7.500488825e-6*x^2 + 2.157426008e-2*x - 19.81881298
  // REV (ADC, W):
  //   1718: 1
  //   3148: 5
  //   4885: 12
  //   7268: 25
  //   11518: 50
  //   17208: 100
  //   eq: y = 4.096215909e-19 * x^5 - 1.432648908e-14 * x^4 + 1.583612334e-10 * x^3 - 3.682819754e-7 * x^2 + 2.508387335e-3 * x - 2.906746954
  //
  // Note: fwd and reverse use different equations due to slightly different readings (possibly
  // due to slight differences in toroid winding). Autotune algorithm should not be impacted
  // as it uses the lowest SWR regardless of what we calculate it to. You may also want to update 
  // the following equations if you're using a different implementation of the SWR bridge.
  forwardPower_ =
     2.322509224e-18 * pow(forwardPowerAdc_, 5) -
     9.260417149e-14 * pow(forwardPowerAdc_, 4) + 
     1.292824426e-9 * pow(forwardPowerAdc_, 3) - 
     7.500488825e-6 * pow(forwardPowerAdc_, 2) + 
     2.157426008e-2 * forwardPowerAdc_ - 
     19.81881298;
  revPower_ = 
     4.096215909e-19 * pow(revPowerAdc_, 5) - 
     1.432648908e-14 * pow(revPowerAdc_, 4) + 
     1.583612334e-10 * pow(revPowerAdc_, 3) - 
     3.682819754e-7 * pow(revPowerAdc_, 2) + 
     2.508387335e-3 * revPowerAdc_ - 
     2.906746954;
  
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
