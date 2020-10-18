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
  if (revPowerAdc_ < 0)
  {
    revPowerAdc_ = 0;
  }
  if (forwardPowerAdc_ < 0)
  {
    forwardPowerAdc_ = 0;
  }
  
  forwardPower_ = fwdPowerFromAdc_(forwardPowerAdc_);
  revPower_ = revPowerFromAdc_(revPowerAdc_);

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

double PowerMonitor::fwdPowerFromAdc_(int16_t adc)
{
  // Truncate "low" ADC values to zero due to issues with below curve fitting.
  if (adc < 500) return 0.0;
  
  // Polynominal fitting @ degree 5 based on calculations at http://www.xuru.org/rt/PR.asp
  // and the following data:
  //
  //   1843 1
  //   3187 5
  //   4853 12
  //   6632 25
  //   10157 50
  //   14900 100
  //   eq: y = 3.377917497e-18 * x^5 - 1.18770898e-1 * x^4 + 1.46712663e-9*x^3 - 7.423572601e-6*x^2 + 1.913878053e-2*x - 16.94328854
  //
  // Note: fwd and reverse use different equations due to slightly different readings (possibly
  // due to slight differences in toroid winding). Autotune algorithm should not be impacted
  // as it uses the lowest SWR regardless of what we calculate it to. You may also want to update 
  // the following equations if you're using a different implementation of the SWR bridge.
  return abs(
    3.377917497e-18 * pow(adc, 5) -
    1.18770898e-13 * pow(adc, 4) + 
    1.46712663e-9 * pow(adc, 3) - 
    7.423572601e-6 * pow(adc, 2) + 
    1.913878053e-2 * adc - 
    16.94328854);
}

double PowerMonitor::revPowerFromAdc_(int16_t adc)
{
  // Truncate "low" ADC values to zero due to issues with below curve fitting.
  if (adc < 500) return 0.0;
  
  // Polynominal fitting @ degree 5 based on calculations at http://www.xuru.org/rt/PR.asp
  // and the following data:
  //
  //   1079 1
  //   1288 5
  //   1444 12
  //   2046 25
  //   3628 50
  //   5910 100
  //   eq: y = -9.967647711e-15 * x^5 + 1.403077787e-10 * x^4 - 7.101543076e-7*x^3 + 1.620120307e-3*x^2 - 1.662151832*x + 624.7554526
  //
  // Note: fwd and reverse use different equations due to slightly different readings (possibly
  // due to slight differences in toroid winding). Autotune algorithm should not be impacted
  // as it uses the lowest SWR regardless of what we calculate it to. You may also want to update 
  // the following equations if you're using a different implementation of the SWR bridge.
  return abs(
    -9.967647711e-15 * pow(adc, 5) +
    1.403077787e-10 * pow(adc, 4) -
    7.101543076e-7 * pow(adc, 3) + 
    1.620120307e-3 * pow(adc, 2) - 
    1.662151832 * adc +
    624.7554526);
}
