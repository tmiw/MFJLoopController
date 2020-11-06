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

  void nextAdc() { ads1015_.readNextAdc(); }

  void enableAdc(bool enable) { ads1015_.enableAdc(enable); }
  
private:
  class ADS1115_Modified : public Adafruit_ADS1115 
  {
      public:
        ADS1115_Modified(uint8_t i2cAddress = ADS1015_ADDRESS) 
        : Adafruit_ADS1115(i2cAddress)
        , currentChannel_(0)
        , active_(false) {
          currentAdc_[0] = 0;
          currentAdc_[1] = 0; 
          tempAdc_[0] = 0;
          tempAdc_[1] = 0; 
        }

        uint16_t beginReadADC()
        {
          currentChannel_ = 0;
          active_ = true;
          startAdcRead_();
        }

        void readNextAdc()
        {
          // Read the conversion results
          // Shift 12-bit results right 4 bits for the ADS1015
          Wire.beginTransmission(m_i2cAddress);
          Wire.write(ADS1015_REG_POINTER_CONVERT);
          Wire.endTransmission();
          Wire.requestFrom(m_i2cAddress, (uint8_t)2);
          tempAdc_[currentChannel_] = ((Wire.read() << 8) | Wire.read()) >> m_bitShift;

          if (currentChannel_ == 1)
          {
            // Copy temporary ADC values to current.
            currentAdc_[0] = tempAdc_[0];
            currentAdc_[1] = tempAdc_[1];
          }

          currentChannel_ = (currentChannel_ + 1) % 2;

          if (active_)
          {
            startAdcRead_();
          }
        }
        
        int16_t getAdcValue(int channel) { return currentAdc_[channel]; }

        void enableAdc(bool enable) {
          if (!active_ && enable)
          {
            startAdcRead_();
          }

          active_ = enable;
        }
        
    private:
      int16_t tempAdc_[2];
      int16_t currentAdc_[2];
      int currentChannel_;
      bool active_;
      
      void startAdcRead_()
      {
        // Start with default values
        uint16_t config =
          ADS1015_REG_CONFIG_CQUE_1CONV |   // Comparator enabled and asserts on 1
                                            // match
          ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching mode
          ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
          ADS1015_REG_CONFIG_CMODE_WINDOW | // Window comparator
          ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 SPS
          ADS1015_REG_CONFIG_MODE_SINGLE;   // Single shot mode.
      
        // Set PGA/voltage range
        config |= m_gain;
      
        // Set single-ended input channel
        switch (currentChannel_) {
        case (0):
          config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1;
          break;
        case (1):
          config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;
          break;
        }
      
        // Set 'start single-conversion' bit
        config |= ADS1015_REG_CONFIG_OS_SINGLE;

        // Update thresholds
        Wire.beginTransmission(m_i2cAddress);
        Wire.write((uint8_t)ADS1015_REG_POINTER_HITHRESH);
        Wire.write(0x80);
        Wire.write(0x00);
        Wire.endTransmission();

        Wire.beginTransmission(m_i2cAddress);
        Wire.write((uint8_t)ADS1015_REG_POINTER_LOWTHRESH);
        Wire.write(0x00);
        Wire.write(0x00);
        Wire.endTransmission();
        
        // Write config register to the ADC
        Wire.beginTransmission(m_i2cAddress);
        Wire.write((uint8_t)ADS1015_REG_POINTER_CONFIG);
        Wire.write((uint8_t)(config >> 8));
        Wire.write((uint8_t)(config & 0xFF));
        Wire.endTransmission();
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
