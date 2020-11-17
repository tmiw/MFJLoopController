#include "CapacitorController.h"
#include "config.h"

CapacitorController::CapacitorController()
: direction_( NONE )
, speed_( IDLE )
, pwmState_( DISABLED )
, pwmPin_( 0 )
, pwmHighTimeMs_( 0 )
, pwmLowTimeMs_( 0 )
, pwmStateStartTime_( 0 )
{
  // empty
}

CapacitorController::~CapacitorController()
{
  // empty
}

void CapacitorController::setup()
{
  // Initialize both tuning pins to low.
  pinMode(TUNE_UP_PIN, OUTPUT);
  pinMode(TUNE_DOWN_PIN, OUTPUT);
  pinMode(TUNE_LED_PIN, OUTPUT);
  disableTuningPins_();
}

void CapacitorController::setDirection(Direction dir)
{
  // Force both tuning pins to low in the event of a direction change
  // to guarantee that the system doesn't short out.
  disableTuningPins_();
  pwmState_ = DISABLED;
  pwmStateStartTime_ = 0;
  
  // Set direction. Next call to process() will set the needed pin to high.
  direction_ = dir;
}

CapacitorController::Direction CapacitorController::getDirection() const
{
  return direction_;
}

void CapacitorController::setSpeed(CapacitorController::Speed speed)
{
  speed_ = speed;
}

CapacitorController::Speed CapacitorController::getSpeed() const
{
  return speed_;
}

void CapacitorController::process()
{
  // Set next PWM state.
  if (speed_ == IDLE)
  {
    disableTuningPins_();
    pwmState_ = DISABLED;
  }
  else if (speed_ == FINE)
  {
    setPwmSettings(direction_ == DOWN ? TUNE_DOWN_PIN : TUNE_UP_PIN, TUNE_FINE_PULSE_ON_TIME_MS, TUNE_FINE_PULSE_OFF_TIME_MS);
  }
  else if (speed_ == SLOW)
  {
    setPwmSettings(direction_ == DOWN ? TUNE_DOWN_PIN : TUNE_UP_PIN, TUNE_SLOW_PULSE_ON_TIME_MS, TUNE_SLOW_PULSE_OFF_TIME_MS);
  }
  else if (speed_ == FAST)
  {
    setPwmSettings(direction_ == DOWN ? TUNE_DOWN_PIN : TUNE_UP_PIN, TUNE_FAST_PULSE_ON_TIME_MS, TUNE_FAST_PULSE_OFF_TIME_MS);
  }

  // PWM handling
  if (pwmState_ != DISABLED)
  {
    auto currentTime = millis();
    if (pwmStateStartTime_ == 0) pwmStateStartTime_ = currentTime;
    
    switch (pwmState_)
    {
      case PWM_HIGH:
        //GPOC = (1 << TUNE_LED_PIN);
        //GPOS = (1 << pwmPin_);
        digitalWrite(TUNE_LED_PIN, LOW);
        digitalWrite(pwmPin_, HIGH);

        if ((currentTime - pwmStateStartTime_) >= pwmHighTimeMs_)
        {
          if (pwmLowTimeMs_ > 0)
          {
            disableTuningPins_();
            pwmState_ = PWM_LOW;
          }
          pwmStateStartTime_ = 0; // Will reset to the current time on next loop
        }
        break;
      case PWM_LOW:
        //GPOS = (1 << TUNE_LED_PIN);
        //GPOC = (1 << pwmPin_);
        digitalWrite(TUNE_LED_PIN, HIGH);
        digitalWrite(pwmPin_, LOW);

        if ((currentTime - pwmStateStartTime_) >= pwmLowTimeMs_)
        {
          disableTuningPins_();
          pwmState_ = PWM_HIGH;
          pwmStateStartTime_ = 0; // Will reset to the current time on next loop
        }
        break;
    }
  }
}

void CapacitorController::disableTuningPins_()
{
  // Directly manipulating the needed registers is far faster than using
  // digitalWrite() and thus will reduce the overshoot when switching directions.
  //GPOC = (1 << TUNE_UP_PIN) | (1 << TUNE_DOWN_PIN);
  //GPOS = (1 << TUNE_LED_PIN);
  digitalWrite(TUNE_UP_PIN, LOW);
  digitalWrite(TUNE_DOWN_PIN, LOW);
  digitalWrite(TUNE_LED_PIN, HIGH);
}

void CapacitorController::forceStop()
{
  disableTuningPins_();
  speed_ = IDLE;
  direction_ = NONE;
  pwmState_ = DISABLED;
}

void CapacitorController::setPwmSettings(int pin, int highTimeMs, int lowTimeMs)
{
  if (pwmState_ == DISABLED || pin != pwmPin_ || highTimeMs != pwmHighTimeMs_ || lowTimeMs != pwmLowTimeMs_)
  {
    disableTuningPins_();
    pwmPin_ = pin;
    pwmHighTimeMs_ = highTimeMs;
    pwmLowTimeMs_ = lowTimeMs;
    pwmState_ = PWM_HIGH;
    pwmStateStartTime_ = 0; // will reset on next loop
  }
}
