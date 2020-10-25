#if !defined(CONFIG_H)
#define CONFIG_H

#include <ESP8266WiFi.h>

#define STASSID "***REMOVED***"
#define STAPSK  "***REMOVED***"

#define MDNS_NAME "mfj-loop"

#define TUNE_UP_PIN D0
#define TUNE_DOWN_PIN D5
#define ADC_INTERRUPT_PIN D6
#define TUNE_LED_PIN D4

// ADC gain
#define ADC_GAIN GAIN_ONE

// ADC channels
#define ADC_FWD_CH 0
#define ADC_REV_CH 2

// Slow tuning pulse time.
#define TUNE_PULSE_OFF_TIME_MS 100
#define TUNE_PULSE_ON_TIME_MS 20

// VSWR "tuning complete" threshold (above which we switch to slow tuning).
#define FAST_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)2.0)

#endif // CONFIG_H
