#include <Wire.h>
#include "PowerMonitor.h"
#include "config.h"

static PowerMonitor* StaticPowerMonitor;

PowerMonitor::PowerMonitor()
: forwardPower_(0.0)
, forwardPowerAdc_(0)
, revPower_(0.0)
, revPowerAdc_(0)
, vswr_(0.0)
{
  StaticPowerMonitor = this;
}

PowerMonitor::~PowerMonitor()
{
  // empty
}

ICACHE_RAM_ATTR void handleAdcAlertInterrupt()
{
  StaticPowerMonitor->nextAdc();
}

void PowerMonitor::setup()
{
  // Set up ADC object.
  ads1015_.begin();
  ads1015_.setGain(ADC_GAIN);
  pinMode(ADC_INTERRUPT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ADC_INTERRUPT_PIN), handleAdcAlertInterrupt, FALLING);

  // Start background ADC reads.
  ads1015_.beginReadADC();
}

void PowerMonitor::process()
{
  int16_t tmpFwdAdc = ads1015_.getAdcValue(0); // ads1015_.readADC_SingleEnded(ADC_FWD_CH);
  int16_t tmpRevAdc = ads1015_.getAdcValue(1); // ads1015_.readADC_SingleEnded(ADC_REV_CH);

  // LSB of ADC value is still subject to noise after front end buffering/LPF, round based on it.
  forwardPowerAdc_ = tmpFwdAdc; // (tmpFwdAdc & 0xFFFE) + ((tmpFwdAdc & 1) << 1);
  revPowerAdc_ = tmpRevAdc; //(tmpRevAdc & 0xFFFE) + ((tmpRevAdc & 1) << 1);
      
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
    double coeff = sqrt((double)revPower_ / (double)forwardPower_);
    if (coeff == 1.0)
    {
      vswr_ = 999.9;
    }
    else
    {
      vswr_ = (1.0 + coeff) / (1.0 - coeff);
    }
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
