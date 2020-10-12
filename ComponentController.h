#if !defined(COMPONENT_CONTROLLER_H)
#define COMPONENT_CONTROLLER_H

class ComponentController
{
public:
  virtual void process() = 0;
  virtual void setup() = 0;

protected:
  ComponentController() { /* empty */ }
  virtual ~ComponentController() { /* empty */ }
};

#endif // COMPONENT_CONTROLLER_H
