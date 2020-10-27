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
  uint32_t tmpFwdAdc = 0;
  uint32_t tmpRevAdc = 0;
  
  for (auto i = 0; i < 16; i++)
  {
      tmpFwdAdc += ads1015_.readADC_Continuous(ADC_FWD_CH);
      tmpRevAdc += ads1015_.readADC_Continuous(ADC_REV_CH);
  }

  forwardPowerAdc_ = tmpFwdAdc / 16;
  revPowerAdc_ = tmpRevAdc / 16;
  
  // IIR filtration for each ADC value.
  /*if (forwardPowerAdc_ == 0)
  {
    c = tmpAdcFwd;
  }
  else
  {
    forwardPowerAdc_ += (tmpAdcFwd - forwardPowerAdc_) / 16;
  }

  if (revPowerAdc_ == 0)
  {
    revPowerAdc_ = tmpAdcRev;
  }
  else
  {
    revPowerAdc_ += (tmpAdcRev - revPowerAdc_) / 16;
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
  
  // Polynominal fitting @ degree 5 based on calculations at https://arachnoid.com/polysolve/
  // and the following data:
  //
  //   12 0
  //   2361 5
  //   3915 10
  //   4595 15
  //   5378 20
  //   6071 25
  //   6764 30
  //   7533 35
  //   8221 40
  //   8980 45
  //   9591 50
  //   10095 55
  //   10606 60
  //   11101 65
  //   11604 70
  //   12109 75
  //   12518 80
  //   12931 85
  //   13339 90
  //   13830 95
  //   14241 100
  //   eq: y = -4.0017073502796845e-019 x^5 + 1.6638800114614116e-014 x^4 - 2.4968122806285582e-010 x^3 + 1.9615755904512023e-006 x^2 - 1.8702860105643696e-003 x + 1.7639189610096823e-001
  //
  // Note: fwd and reverse use different equations due to slightly different readings (possibly
  // due to slight differences in toroid winding). Autotune algorithm should not be impacted
  // as it uses the lowest SWR regardless of what we calculate it to. You may also want to update 
  // the following equation if you're using a different implementation of the SWR bridge.
  return abs(
    -4.0017073502796845e-019 * pow(adc, 5) +
    1.6638800114614116e-014 * pow(adc, 4) - 
    2.4968122806285582e-010 * pow(adc, 3) + 
    1.9615755904512023e-006 * pow(adc, 2) - 
    1.8702860105643696e-003 * adc + 
    1.7639189610096823e-001);
}
