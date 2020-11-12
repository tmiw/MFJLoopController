#if !defined(CONFIG_H)
#define CONFIG_H

#include <ESP8266WiFi.h>

// Define STASSID (your wireless SSID) and STAPSK (your network's password)
// inside config-wifi.h. The below file is in .gitignore to prevent accidental check-in.
#include "config-wifi.h"

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

// Fine tuning pulse time.
#define TUNE_FINE_PULSE_OFF_TIME_MS 150
#define TUNE_FINE_PULSE_ON_TIME_MS 20

// Slow tuning pulse time.
#define TUNE_SLOW_PULSE_OFF_TIME_MS 115
#define TUNE_SLOW_PULSE_ON_TIME_MS 36

// Fast tuning pulse time.
#define TUNE_FAST_PULSE_OFF_TIME_MS 0
#define TUNE_FAST_PULSE_ON_TIME_MS 36

// Length of time in milliseconds to wait after detecting SWR dip before auto-tuner
// proceeds with the next state. This is because the antenna will continue moving even
// after power is removed, so we want to ensure that we go through all of the necessary
// tuning states.
#define AUTOTUNE_STATE_WAIT_TIME_MS 250

// VSWR "tuning complete" threshold (below after which we switch to slow tuning).
#define FAST_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)3.0)

// VSWR "tuning complete" threshold (below after which we stop tuning).
#define SLOW_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)2.0)

#endif // CONFIG_H
