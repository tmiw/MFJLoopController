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
#define TUNE_FINE_PULSE_ON_TIME_MS 18

// Slow tuning pulse time.
#define TUNE_SLOW_PULSE_OFF_TIME_MS 115
#define TUNE_SLOW_PULSE_ON_TIME_MS 36

// Fast tuning pulse time.
#define TUNE_FAST_PULSE_OFF_TIME_MS 0
#define TUNE_FAST_PULSE_ON_TIME_MS 36

// Minimum SWR difference (from prior reading) to wait for while in a wait state
// before proceeding to the next state.
#define AUTOTUNE_MIN_SWR_DIFF_IN_WAIT 0.1

// Length of time to move in the opposite direction before beginning auto-tune. This
// time is to ensure that we pass the dip while in fast mode (instead of stopping just
// short of it).
#define AUTOTUNE_BEGIN_TUNE_TIME_MS 2000

// VSWR "tuning complete" threshold (below after which we switch to slow tuning).
#define FAST_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)2.0)

// VSWR "tuning complete" threshold (below after which we stop tuning).
#define SLOW_AUTOTUNE_COMPLETE_VSWR_THRESH ((double)2.0)

// Minimum wait time between tuning states to ensure capacitor remains stable enough
#define MIN_TUNE_WAIT_TIME_MS 100

// Maximum "good enough" SWR to exit tuning.
#define MAX_GOOD_SWR_END_TUNING ((double)1.5)

#endif // CONFIG_H
