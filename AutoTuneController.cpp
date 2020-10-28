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
, curIdx_(0)
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

  // Clear SWR list
  clearSWRList();
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
  //double revPower = pPowerMonitor_->getReversePowerADC();

  // Add SWR reading to circular buffer.
  addSWRReading(swr);
  
  switch(currentState_)
  {
    case BEGIN_TUNE:
      currentState_ = SLOW_TUNE; // TODO: figure out why starting with FAST_TUNE is unreliable
      minSWRVal_ = DBL_MAX;
      break;
    //case FAST_TUNE:
    case SLOW_TUNE:
    case TOP_UP:     
      pCapacitorController_->setDirection(getDirectionForCurrentState_());
      pCapacitorController_->setSpeed(getSpeedForCurrentState_());

      #if 0
      if (swr <= getSWRThresholdForCurrentState_() && swr <= minSWRVal_)
      {
        minSWRVal_ = swr;
      }
      else if (minSWRVal_ != DBL_MAX)
      {
        if (swr > minSWRVal_ /*&& swr >= getSWRThresholdForCurrentState_()*/)
        {
          pCapacitorController_->forceStop(); // to reduce the amount of distance to cover in slow mode.
          currentState_ = getNextState_();
          minSWRVal_ = DBL_MAX;
        }
      }
      #endif

      if (isPastPeak())
      {
        pCapacitorController_->forceStop(); // to reduce the amount of distance to cover in slow mode.
        clearSWRList();
        currentState_ = getNextState_();
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

CapacitorController::Direction AutoTuneController::getDirectionForCurrentState_() const
{
  switch(currentState_)
  {
    //case FAST_TUNE:
    case SLOW_TUNE:
      return autoTuneDirection_ == DOWN ? CapacitorController::DOWN : CapacitorController::UP;
    case TOP_UP:
      return autoTuneDirection_ == DOWN ? CapacitorController::UP : CapacitorController::DOWN;
    default:
      return CapacitorController::NONE;
  }
}

CapacitorController::Speed AutoTuneController::getSpeedForCurrentState_() const
{
  switch(currentState_)
  {
    /*case FAST_TUNE:
      return CapacitorController::FAST;*/
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
    //case FAST_TUNE:
    case SLOW_TUNE:
      return FAST_AUTOTUNE_COMPLETE_VSWR_THRESH;
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
    /*case FAST_TUNE:
      return SLOW_TUNE;*/
    case SLOW_TUNE:
      return TOP_UP;
    case TOP_UP:
    default:
      return IDLE;
  }
}

bool AutoTuneController::isPastPeak() const
{
  double lastSWR = lastSWRVals_[curIdx_];
  double firstSWR = 0.0;
  auto index = (curIdx_ + 1) % NUM_SWR_VALS;

  // Look for the first non-zero value in the list.
  for (auto cnt = 0; cnt < NUM_SWR_VALS - 1; cnt++)
  {
    firstSWR = lastSWRVals_[index];
    if (firstSWR > 0.0) 
    {
      break;
    }
    else 
    {
      index = (index + 1) % NUM_SWR_VALS;
    }
  }

  // If there's only one data item in this list, we definitely haven't past a peak yet.
  if (firstSWR == 0.0)
  {
    return false;
  }

  // Now cycle through the list again, starting from the item just after the first one
  // we found through the item just before the last one. If we find a value such that
  // it's both less than the first and last (e.g. a possible peak), return true.
  index = (index + 1) % NUM_SWR_VALS;
  for (auto cnt = 0; cnt < NUM_SWR_VALS - 2; cnt++)
  {
    if (index == curIdx_) break;
    
    double currentSWR = lastSWRVals_[index];
    double diffFromFirst = firstSWR - currentSWR;
    double diffFromLast = lastSWR - currentSWR;

    if (diffFromFirst > 0.0 && diffFromLast > 0.0)
    {
      return true;
    }
    else
    {
      index = (index + 1) % NUM_SWR_VALS;
    }
  }

  return false;
}

void AutoTuneController::addSWRReading(double swr)
{
  curIdx_ = (curIdx_ + 1) % NUM_SWR_VALS;
  lastSWRVals_[curIdx_] = swr;
}
 
void AutoTuneController::clearSWRList()
{
  for (auto index = 0; index < NUM_SWR_VALS; index++)
  {
    lastSWRVals_[index] = 0.0;
  }
  curIdx_ = 0;
} 
