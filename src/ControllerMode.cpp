#include "ControllerMode.hpp"
#include <Arduino.h>

ControllerMode::ControllerMode(LCDManager *l, UARTManager *u)
    : lcd(l), uart(u), active(false), lastSend(0) {}

void ControllerMode::start() {
  active = true;
  lcd->clear();
  lcd->printLine(0, "CYBER RUNNER");
  lcd->printLine(1, "Ready...");
}

void ControllerMode::update() {
  if (!active)
    return;

  // High speed button check
  // We don't use InputManager's debounce here because we want raw speed/hold
  // detection But for simplicity, we'll use InputManager and just send on
  // change or repeat

  Button btn = InputManager::readButton();

  // Throttle sending slightly to avoid flooding serial buffer
  if (millis() - lastSend > 50) {
    if (btn == BTN_UP) {
      uart->sendResponse("CTRL:JUMP");
      lastSend = millis();
    } else if (btn == BTN_DOWN) {
      uart->sendResponse("CTRL:DUCK");
      lastSend = millis();
    }
  }
}

bool ControllerMode::isActive() { return active; }

void ControllerMode::stop() { active = false; }
