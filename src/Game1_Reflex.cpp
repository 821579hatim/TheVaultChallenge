#include "Game1_Reflex.hpp"
#include <Arduino.h>

Game1_Reflex::Game1_Reflex(LCDManager *l, UARTManager *u)
    : lcd(l), uart(u), active(false), reactionLimit(2000) {}

void Game1_Reflex::start() {
  active = true;
  lcd->clear();
  lcd->printLine(0, "Game 1: Reflex");
  lcd->printLine(1, "Get Ready...");

  // Random delay handled by main loop or here non-blocking?
  // For simplicity, we'll pick a target button immediately but maybe wait a bit
  // In a real non-blocking loop, we'd use a state machine.
  // Here we'll just set the target and start timer.

  // Pick random button (UP, DOWN, LEFT, RIGHT)
  int r = millis() % 4;
  switch (r) {
  case 0:
    targetButton = BTN_UP;
    uart->sendLog("G1_TARGET:UP");
    lcd->printLine(1, "PRESS UP!");
    break;
  case 1:
    targetButton = BTN_DOWN;
    uart->sendLog("G1_TARGET:DOWN");
    lcd->printLine(1, "PRESS DOWN!");
    break;
  case 2:
    targetButton = BTN_LEFT;
    uart->sendLog("G1_TARGET:LEFT");
    lcd->printLine(1, "PRESS LEFT!");
    break;
  case 3:
    targetButton = BTN_RIGHT;
    uart->sendLog("G1_TARGET:RIGHT");
    lcd->printLine(1, "PRESS RIGHT!");
    break;
  }

  startTime = TimerUtils::millis();
}

void Game1_Reflex::update() {
  if (!active)
    return;

  if (TimerUtils::millis() - startTime > reactionLimit) {
    uart->sendResponse("GAME1_FAIL");
    lcd->printLine(1, "Too Slow!");
    active = false;
    return;
  }

  Button pressed = InputManager::readButton();
  if (pressed != BTN_NONE && pressed != BTN_SELECT) {
    if (pressed == targetButton) {
      unsigned long reactionTime = TimerUtils::millis() - startTime;
      uart->sendResponse("GAME1_OK:" + String(reactionTime));
      lcd->printLine(1, "Nice! " + String(reactionTime) + "ms");
      delay(1000);
    } else {
      lcd->printLine(1, "Wrong Button!");
    }
    active = false;
  }
}

bool Game1_Reflex::isActive() { return active; }

void Game1_Reflex::stop() { active = false; }
