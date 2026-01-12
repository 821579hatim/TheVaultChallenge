#ifndef ADC_MANAGER_HPP
#define ADC_MANAGER_HPP

#include <Arduino.h>

#include "libADC.hpp"
#include <Arduino.h>

enum Button { BTN_NONE, BTN_RIGHT, BTN_UP, BTN_DOWN, BTN_LEFT, BTN_SELECT };

class InputManager {
public:
  static void init();
  static Button readButton();
  static int readPotentiometer();
};

#endif
