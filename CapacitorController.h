#if !defined(CAPACITOR_CONTROLLER_H)
#define CAPACITOR_CONTROLLER_H

#include "ComponentController.h"

class CapacitorController : public ComponentController
{
public:
  enum Direction { NONE, DOWN, UP };
  enum Speed { IDLE, FAST, SLOW, FINE };

  CapacitorController();
  virtual ~CapacitorController();

  void setDirection(Direction dir);
  Direction getDirection() const;
  void setSpeed(Speed speed);
  Speed getSpeed() const;
  void onlyOnce(bool once);
  virtual void process();
  virtual void setup();

  void forceStop();
  
private:  
  Direction direction_;  
  Direction prevDirection_;
  Speed speed_;
  bool onlyOnce_;

  void enableTuningPin_();
  void disableTuningPins_();
};

#endif // CAPACITOR_CONTROLLER_H
