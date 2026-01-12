#include "Game2_Memory.hpp"
#include <Arduino.h>

Game2_Memory::Game2_Memory(LCDManager *l, UARTManager *u)
    : lcd(l), uart(u), active(false) {}

void Game2_Memory::start(String sequence) {
  active = true;
  targetSequence = sequence;
  currentStep = 0;
  inputIndex = 0;
  waitingForInput = false;
  showingStep = false;
  lastStepTime = millis();

  lcd->clear();
  lcd->printLine(0, "Game 2: Memory");
  lcd->printLine(1, "Watch closely...");
  delay(1500);
}

void Game2_Memory::displayDirection(char dir) {
  lcd->clear();
  lcd->printLine(0, "Remember:");
  switch (dir) {
  case 'U':
    uart->sendLog("G2_SHOW:UP");
    lcd->printLine(1, "   UP   ");
    break;
  case 'D':
    uart->sendLog("G2_SHOW:DOWN");
    lcd->printLine(1, "  DOWN  ");
    break;
  case 'L':
    uart->sendLog("G2_SHOW:LEFT");
    lcd->printLine(1, "  LEFT  ");
    break;
  case 'R':
    uart->sendLog("G2_SHOW:RIGHT");
    lcd->printLine(1, "  RIGHT ");
    break;
  default:
    lcd->printLine(1, "   ?    ");
    break;
  }
}

void Game2_Memory::update() {
  if (!active)
    return;

  if (!waitingForInput) {
    // Display Phase
    unsigned long now = millis();
    if (now - lastStepTime > 1000) {
      lastStepTime = now;
      if (showingStep) {
        // Clear screen between steps
        lcd->clear();
        showingStep = false;
        currentStep++;
        if (currentStep >= (int)targetSequence.length()) {
          // Done displaying
          waitingForInput = true;
          lcd->printLine(0, "Your Turn!");
          lcd->printLine(1, "Repeat it!");
        }
      } else {
        // Show next step
        if (currentStep < (int)targetSequence.length()) {
          displayDirection(targetSequence[currentStep]);
          showingStep = true;
        }
      }
    }
  } else {
    // Input Phase
    Button btn = InputManager::readButton();
    if (btn != BTN_NONE && btn != BTN_SELECT) {
      char expected = targetSequence[inputIndex];
      bool correct = false;

      if (btn == BTN_UP && expected == 'U')
        correct = true;
      else if (btn == BTN_DOWN && expected == 'D')
        correct = true;
      else if (btn == BTN_LEFT && expected == 'L')
        correct = true;
      else if (btn == BTN_RIGHT && expected == 'R')
        correct = true;

      if (correct) {
        lcd->printLine(1, "Good...");
        inputIndex++;
        if (inputIndex >= (int)targetSequence.length()) {
          uart->sendResponse("GAME2_OK");
          lcd->printLine(0, "CORRECT!");
          lcd->printLine(1, "Well done!");
          delay(1000);
          active = false;
        }
        // Debounce/Wait for release could be needed, but InputManager might
        // handle simple press
        delay(300); // Simple debounce
      } else {
        uart->sendResponse("GAME2_FAIL");
        lcd->printLine(0, "WRONG!");
        lcd->printLine(1, "Game Over");
        delay(1000);
        active = false;
      }
    }
  }
}

bool Game2_Memory::isActive() { return active; }

void Game2_Memory::stop() { active = false; }
