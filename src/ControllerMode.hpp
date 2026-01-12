#ifndef CONTROLLER_MODE_HPP
#define CONTROLLER_MODE_HPP

#include <Arduino.h>

#include "ADCManager.hpp"
#include "LCDManager.hpp"
#include "UARTManager.hpp"
#include <Arduino.h>

class ControllerMode {
private:
  LCDManager *lcd;
  UARTManager *uart;
  bool active;
  unsigned long lastSend;

public:
  ControllerMode(LCDManager *l, UARTManager *u);
  void start();
  void update();
  bool isActive();
  void stop();
};

#endif
