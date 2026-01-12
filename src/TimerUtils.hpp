#ifndef TIMER_UTILS_HPP
#define TIMER_UTILS_HPP

#include <TimerOne.h>

class TimerUtils {
public:
  static void init();
  static void setPeriod(unsigned long microseconds);
  static void attachInterrupt(void (*isr)());
  static void detachInterrupt();
  static void start();
  static void stop();
  // Helper for measuring elapsed time
  static unsigned long millis();
  static unsigned long micros();
};

#endif
