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
  class ADS1115_Modified : public Adafruit_ADS1115 
  {
      public:
        ADS1115_Modified(uint8_t i2cAddress = ADS1015_ADDRESS) : Adafruit_ADS1115(i2cAddress) { }

        uint16_t readADC_Continuous(uint8_t channel)
        {
          // Start with default values
          uint16_t config =
              ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
              ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
              ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
              ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
              ADS1015_REG_CONFIG_DR_3300SPS |   // 3300 samples per second
              ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous mode
        
          // Set PGA/voltage range
          config |= m_gain;
        
          // Set single-ended input channel
          switch (channel) {
          case (0):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
            break;
          case (1):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
            break;
          case (2):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
            break;
          case (3):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
            break;
          }
        
          // Set 'start single-conversion' bit
          config |= ADS1015_REG_CONFIG_OS_SINGLE;
        
          // Write config register to the ADC
          Wire.beginTransmission(m_i2cAddress);
          Wire.write((uint8_t)ADS1015_REG_POINTER_CONFIG);
          Wire.write((uint8_t)(config >> 8));
          Wire.write((uint8_t)(config & 0xFF));
          Wire.endTransmission();
                  
          // Wait for the conversion to complete
          delay(m_conversionDelay);
        
          // Read the conversion results
          // Shift 12-bit results right 4 bits for the ADS1015
          Wire.beginTransmission(m_i2cAddress);
          Wire.write(ADS1015_REG_POINTER_CONVERT);
          Wire.endTransmission();
          Wire.requestFrom(m_i2cAddress, (uint8_t)2);
          return ((Wire.read() << 8) | Wire.read()) >> m_bitShift;
        }
  };
  
  ADS1115_Modified ads1015_;
  double forwardPower_;
  int16_t forwardPowerAdc_;
  double revPower_;
  int16_t revPowerAdc_;
  double vswr_;

  double powerFromAdc_(int16_t adc);
};

#endif // POWER_MONITOR_H
