#if !defined(AUTO_TUNE_CONTROLLER_H)
#define AUTO_TUNE_CONTROLLER_H

#include "ComponentController.h"
#include "CapacitorController.h"

class PowerMonitor;

class AutoTuneController : public ComponentController
{
public:
  enum Direction { NONE, DOWN, UP };
  enum State { IDLE, BEGIN_TUNE, /*FAST_TUNE,*/ SLOW_TUNE, TOP_UP };

  AutoTuneController(CapacitorController* pCapacitorController, PowerMonitor* pPowerMonitor);
  virtual ~AutoTuneController();

  virtual void process();
  virtual void setup();

  void beginTune(Direction direction);
  void endTune();
  Direction getDirection() const;
  State getState() const;
  
private:
  CapacitorController* pCapacitorController_;
  PowerMonitor* pPowerMonitor_;
  State currentState_;
  Direction autoTuneDirection_;
  double minSWRVal_;
  
  CapacitorController::Direction getDirectionForCurrentState_() const;
  CapacitorController::Speed getSpeedForCurrentState_() const;
  double getSWRThresholdForCurrentState_() const;
  State getNextState_() const;
};

#endif // AUTO_TUNE_CONTROLLER_H
