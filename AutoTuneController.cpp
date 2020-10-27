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
, minSWRVal_(DBL_MAX)
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

  // Truncate approximately all digits past the tenths place; too much variation in
  // smaller portions of SWR to be useful for autotuning.
  double swr = trunc(pPowerMonitor_->getVSWR() * 10) / 10;
  
  switch(currentState_)
  {
    case BEGIN_TUNE:
      pCapacitorController_->setDirection(autoTuneDirection_ == DOWN ? CapacitorController::DOWN : CapacitorController::UP);
      pCapacitorController_->setSpeed(CapacitorController::FAST);
      currentState_ = FAST_TUNE;
      minSWRVal_ = DBL_MAX;
      break;
    case FAST_TUNE:
      if (swr <= minSWRVal_)
      {
        minSWRVal_ = swr;
      }
      else
      {
        if (swr > minSWRVal_)
        {
          pCapacitorController_->forceStop(); // to reduce the amount of distance to cover in slow mode.
          currentState_ = TOP_UP;
          minSWRVal_ = DBL_MAX;
        }
      }
      break;
    case TOP_UP:
      pCapacitorController_->setDirection(autoTuneDirection_ == DOWN ? CapacitorController::UP : CapacitorController::DOWN);
      pCapacitorController_->setSpeed(CapacitorController::SLOW);
      if (minSWRVal_ < swr)
      {
        // Autotune complete.
        endTune();
      }
      else
      {
        minSWRVal_ = swr;
      }
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
