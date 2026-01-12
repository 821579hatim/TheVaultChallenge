#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <Arduino.h>

class Interrupts {
public:
  static void init();
  static void attachButtonInterrupt(void (*isr)());
  static void detachButtonInterrupt();
};

#endif
