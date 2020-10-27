#if !defined(CONFIG_H)
#define CONFIG_H

#include <ESP8266WiFi.h>

#define STASSID "YOUR SSID HERE"
#define STAPSK  "YOUR PASSWORD HERE"

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

// Length of initial "long" pulse when changing directions. This is needed
// to overcome inertia/resistance in the capacitor prior to slow tuning.
// Note: this long pulse is not initially performed on boot due to not knowing
// which direction the antenna was moved during the last session. Having the
// pulse be enabled could result in going very far off course.
#define TUNE_INERTIA_PULSE_ON_TIME_MS 500

// VSWR "tuning complete" threshold (above which we switch to slow tuning).
#define FAST_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)2.0)

#endif // CONFIG_H
