#include "TimerUtils.hpp"
#include <Arduino.h>

void TimerUtils::init() {
  Timer1.initialize(1000000); // Default 1s
}

void TimerUtils::setPeriod(unsigned long microseconds) {
  Timer1.setPeriod(microseconds);
}

void TimerUtils::attachInterrupt(void (*isr)()) { Timer1.attachInterrupt(isr); }

void TimerUtils::detachInterrupt() { Timer1.detachInterrupt(); }

void TimerUtils::start() { Timer1.start(); }

void TimerUtils::stop() { Timer1.stop(); }

unsigned long TimerUtils::millis() { return ::millis(); }

unsigned long TimerUtils::micros() { return ::micros(); }
