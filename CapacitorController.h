#if !defined(CAPACITOR_CONTROLLER_H)
#define CAPACITOR_CONTROLLER_H

#include "ComponentController.h"

class CapacitorController : public ComponentController
{
public:
  enum Direction { NONE, DOWN, UP };
  enum Speed { IDLE, FAST, SLOW };

  CapacitorController();
  virtual ~CapacitorController();

  void setDirection(Direction dir);
  Direction getDirection() const;
  void setSpeed(Speed speed);
  Speed getSpeed() const;
  virtual void process();
  virtual void setup();
  
private:
  enum { PULSE_TIME_MS = 50 };
  
  Direction direction_;  
  Speed speed_;

  void enableTuningPin_();
  void disableTuningPins_();
};

#endif // CAPACITOR_CONTROLLER_H
