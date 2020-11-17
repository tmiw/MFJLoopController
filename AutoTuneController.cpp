#include <float.h>
#include "AutoTuneController.h"
#include "CapacitorController.h"
#include "PowerMonitor.h"
#include "config.h"

AutoTuneController::AutoTuneController(CapacitorController* pCapacitorController, PowerMonitor* pPowerMonitor)
: pCapacitorController_(pCapacitorController)
, pPowerMonitor_(pPowerMonitor)
, currentState_(IDLE)
, autoTuneDirection_(NONE)
, waitTimeBegin_(0)
{
  // empty
}

AutoTuneController::~AutoTuneController()
{
  // empty
}

void AutoTuneController::setup()
{
  // empty
}

void AutoTuneController::beginTune(Direction direction)
{
  waitTimeBegin_ = 0;
  currentState_ = BEGIN_TUNE;
  autoTuneDirection_ = direction;
  minSWRVal_ = DBL_MAX;
}

void AutoTuneController::endTune()
{
  pCapacitorController_->forceStop();
  currentState_ = IDLE;
  autoTuneDirection_ = NONE;
}

void AutoTuneController::process()
{
  // Requires RF output to tune. Stop autotuning if it suddenly goes away.
  if (currentState_ != IDLE && pPowerMonitor_->getForwardPower() == 0.0)
  {
    endTune();
    return;
  }

  // Get current SWR.
  double swr = pPowerMonitor_->getVSWR();
  
  switch(currentState_)
  {
    case BEGIN_TUNE:
      // Move in the opposite direction for a bit to guarantee we pass the dip while in fast mode.
      pCapacitorController_->setDirection(getDirectionForCurrentState_());
      pCapacitorController_->setSpeed(getSpeedForCurrentState_());
      
      if (waitTimeBegin_ == 0)
      {
        waitTimeBegin_ = millis();      
      }

      if ((millis() - waitTimeBegin_) >= AUTOTUNE_BEGIN_TUNE_TIME_MS)
      {
        pCapacitorController_->forceStop();
        currentState_ = getNextState_();
        minSWRVal_ = DBL_MAX;
        waitTimeBegin_ = 0;
      }
      break;
    case FAST_TUNE:
    case SLOW_TUNE:
    case TOP_UP:
      if (pCapacitorController_->getDirection() != getDirectionForCurrentState_() ||
          pCapacitorController_->getSpeed() != getSpeedForCurrentState_())
      {
        pCapacitorController_->setDirection(getDirectionForCurrentState_());
        pCapacitorController_->setSpeed(getSpeedForCurrentState_());
      }
      
      if (swr <= getSWRThresholdForCurrentState_() && swr <= minSWRVal_)
      {
        minSWRVal_ = swr;
      }
      
      if (minSWRVal_ != DBL_MAX)
      {
        if (swr > minSWRVal_)
        {
          pCapacitorController_->forceStop(); // to reduce the amount of distance to cover in slow mode.
          currentState_ = getNextState_();
          minSWRVal_ = DBL_MAX;
          waitTimeBegin_ = 0;
        }
      }
      
      break;
    case FAST_TUNE_WAIT:
    case SLOW_TUNE_WAIT:
      // Wait state to allow SWR to stabilize after terminating tuning step.
      if (fabs(minSWRVal_ - swr) <= AUTOTUNE_MIN_SWR_DIFF_IN_WAIT)
      {
        minSWRVal_ = DBL_MAX;
        currentState_ = getNextState_();
      }

      minSWRVal_ = swr;
      break;
    case IDLE:
    default:
      break;
  }
}

AutoTuneController::Direction AutoTuneController::getDirection() const
{
  return autoTuneDirection_;
}

AutoTuneController::State AutoTuneController::getState() const
{
  return currentState_;
}

CapacitorController::Direction AutoTuneController::getDirectionForCurrentState_() const
{
  switch(currentState_)
  {
    case FAST_TUNE:
    case TOP_UP:
      return autoTuneDirection_ == DOWN ? CapacitorController::DOWN : CapacitorController::UP;
    case BEGIN_TUNE:
    case SLOW_TUNE:
      return autoTuneDirection_ == DOWN ? CapacitorController::UP : CapacitorController::DOWN;
    default:
      return CapacitorController::NONE;
  }
}

CapacitorController::Speed AutoTuneController::getSpeedForCurrentState_() const
{
  switch(currentState_)
  {
    case BEGIN_TUNE:
    case FAST_TUNE:
      return CapacitorController::FAST;
    case SLOW_TUNE:
      return CapacitorController::SLOW;
    case TOP_UP:
      return CapacitorController::FINE;
    default:
      return CapacitorController::IDLE;
  }
}

double AutoTuneController::getSWRThresholdForCurrentState_() const
{
  switch(currentState_)
  {
    case FAST_TUNE:
      return FAST_AUTOTUNE_COMPLETE_VSWR_THRESH;
    case SLOW_TUNE:
    case TOP_UP:
      return SLOW_AUTOTUNE_COMPLETE_VSWR_THRESH;
    default:
      return DBL_MAX;
  }
}

AutoTuneController::State AutoTuneController::getNextState_() const
{
  switch(currentState_)
  {
    case BEGIN_TUNE:
      return FAST_TUNE;
    case FAST_TUNE:
      return FAST_TUNE_WAIT;
    case FAST_TUNE_WAIT:
      return SLOW_TUNE;
    case SLOW_TUNE:
      return SLOW_TUNE_WAIT;
    case SLOW_TUNE_WAIT:
      return TOP_UP;
    case TOP_UP:
    default:
      return IDLE;
  }
}
