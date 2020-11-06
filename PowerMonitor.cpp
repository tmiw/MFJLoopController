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
  //   95 0
  //   1490 1
  //   1748 2
  //   2005 3
  //   2185 4
  //   2434 5
  //   3900 10
  //   4683 15
  //   5369 20
  //   6151 25
  //   6839 30
  //   7522 35
  //   8228 40
  //   8998 45
  //   9678 50
  //   10206 55
  //   10703 60
  //   11220 65
  //   11804 70
  //   12219 75
  //   12639 80
  //   13051 85
  //   13466 90
  //   13967 95
  //   14371 100
  //   eq: y = -2.7763298898827872e-019 x^5 + 1.2528824451236192e-014 x^4 - 2.0200694053523373e-010 x^3 + 1.7298629762675664e-006 x^2 - 1.5215893510124429e-003 x + 3.0373673651590188e-001
  //
  // Note: fwd and reverse use different equations due to slightly different readings (possibly
  // due to slight differences in toroid winding). Autotune algorithm should not be impacted
  // as it uses the lowest SWR regardless of what we calculate it to. You may also want to update 
  // the following equation if you're using a different implementation of the SWR bridge.
  return abs(
    -2.7763298898827872e-019 * pow(adc, 5) +
    1.2528824451236192e-014 * pow(adc, 4) - 
    2.0200694053523373e-010 * pow(adc, 3) + 
    1.7298629762675664e-006 * pow(adc, 2) - 
    1.5215893510124429e-003 * adc + 
    3.0373673651590188e-001);
}
