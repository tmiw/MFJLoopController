#if !defined(CONFIG_H)
#define CONFIG_H

#include <ESP8266WiFi.h>

#define STASSID "***REMOVED***"
#define STAPSK  "***REMOVED***"

#define MDNS_NAME "mfj-loop"

#define TUNE_UP_PIN D0
#define TUNE_DOWN_PIN D5
#define ADC_INTERRUPT_PIN D6

// ADC gain
#define ADC_GAIN GAIN_ONE

// ADC channels
#define ADC_FWD_CH 3
#define ADC_REV_CH 1

// Slow tuning pulse time.
#define TUNE_PULSE_TIME_MS 50

// VSWR "tuning complete" threshold (above which we switch to slow tuning).
#define FAST_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)2.0)

#endif // CONFIG_H
