#if !defined(CONFIG_H)
#define CONFIG_H

#include <ESP8266WiFi.h>

#define STASSID "YOUR WIFI NETWORK HERE"
#define STAPSK  "YOUR WIFI PASSWORD HERE"

#define MDNS_NAME "mfj-loop"

#define TUNE_UP_PIN D0
#define TUNE_DOWN_PIN D5
#define ADC_INTERRUPT_PIN D6

// Maximum ADC value for reverse power to trigger transition to slow tuning (TBD)
//#define FAST_TUNE_MAX_ADC 512

// ADC offsets for fwd/reverse (TBD)
#define ADC_FWD_OFFSET 625
#define ADC_REV_MAX_RAW 618
#define ADC_REV_MAX_PO 1.25
#define ADC_REV_OFFSET 566

// ADC gain
#define ADC_GAIN GAIN_EIGHT

// ADC channels
#define ADC_FWD_CH 0
#define ADC_REV_CH 2

// Slow tuning pulse time.
#define TUNE_PULSE_TIME_MS 50

// VSWR "tuning complete" threshold
#define AUTOTUNE_COMPLETE_VSWR_THRESH ((double)1.5)

#endif // CONFIG_H
