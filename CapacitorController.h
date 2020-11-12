#if !defined(CAPACITOR_CONTROLLER_H)
#define CAPACITOR_CONTROLLER_H

#include "ComponentController.h"

class CapacitorController : public ComponentController
{
public:
  enum Direction { NONE, DOWN, UP };
  enum Speed { IDLE, FAST, SLOW, FINE };
  enum State { DISABLED, PWM_HIGH, PWM_LOW };
  
  CapacitorController();
  virtual ~CapacitorController();

  void setDirection(Direction dir);
  Direction getDirection() const;
  void setSpeed(Speed speed);
  Speed getSpeed() const;
  virtual void process();
  virtual void setup();

  void forceStop();
  
private:  
  Direction direction_;  
  Speed speed_;

  // PWM control
  State pwmState_;
  int pwmPin_;
  int pwmHighTimeMs_;
  int pwmLowTimeMs_;
  unsigned long pwmStateStartTime_;
  
  void enableTuningPin_();
  void disableTuningPins_();

  void setPwmSettings(int pin, int highTimeMs, int lowTimeMs);
};

#endif // CAPACITOR_CONTROLLER_H
