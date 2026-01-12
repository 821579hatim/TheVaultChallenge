#include "Interrupts.hpp"

#define BUTTON_INT_PIN 2 // INT0

void Interrupts::init() { pinMode(BUTTON_INT_PIN, INPUT_PULLUP); }

void Interrupts::attachButtonInterrupt(void (*isr)()) {
  attachInterrupt(digitalPinToInterrupt(BUTTON_INT_PIN), isr, FALLING);
}

void Interrupts::detachButtonInterrupt() {
  detachInterrupt(digitalPinToInterrupt(BUTTON_INT_PIN));
}
