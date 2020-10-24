#include "CapacitorController.h"
#include "config.h"

CapacitorController::CapacitorController()
: direction_( NONE )
, speed_( IDLE )
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
  disableTuningPins_();
}

void CapacitorController::setDirection(Direction dir)
{
  // Force both tuning pins to low in the event of a direction change
  // to guarantee that the system doesn't short out.
  digitalWrite(TUNE_UP_PIN, LOW);
  digitalWrite(TUNE_DOWN_PIN, LOW);

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
  if (speed_ == IDLE)
  {
    disableTuningPins_();
  }
  else if (speed_ == SLOW)
  {
    // Pulse tuning pin.
    enableTuningPin_();
    delay(TUNE_PULSE_ON_TIME_MS);
    disableTuningPins_();
    delay(TUNE_PULSE_OFF_TIME_MS);
  }
  else if (speed_ == FAST)
  {
    // Enable needed tuning pin.
    enableTuningPin_();
  }
}

void CapacitorController::enableTuningPin_()
{
  switch(direction_)
  {
    case DOWN:
      digitalWrite(TUNE_DOWN_PIN, HIGH);
      break;
    case UP:
      digitalWrite(TUNE_UP_PIN, HIGH);
      break;
    case NONE:
    default:
      break;
  }
}

void CapacitorController::disableTuningPins_()
{
  digitalWrite(TUNE_UP_PIN, LOW);
  digitalWrite(TUNE_DOWN_PIN, LOW);
}

void CapacitorController::forceStop()
{
  disableTuningPins_();
  speed_ = IDLE;
  direction_ = NONE;
}
