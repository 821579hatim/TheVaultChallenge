#include "QuizModule.hpp"
#include <Arduino.h>

QuizModule::QuizModule(LCDManager *l, UARTManager *u)
    : lcd(l), uart(u), active(false), currentSelection(0),
      waitingForResponse(false), quizId(0) {}

void QuizModule::start(int id) {
  active = true;
  quizId = id;
  currentSelection = 0;
  waitingForResponse = false;
  lcd->clear();
  lcd->printLine(0, "Quiz " + String(id));
  lcd->printLine(1, "Select: 0");
}

void QuizModule::handleResponse(String response) {
  if (response == "CORRECT") {
    lcd->printLine(0, "CORRECT!");
    lcd->printLine(1, "Code Found!");
    delay(1000);
    active = false;
    uart->sendResponse("QUIZ_OK:" + String(quizId));
  } else if (response == "WRONG") {
    lcd->printLine(0, "WRONG!");
    lcd->printLine(1, "Try Again");
    delay(1000);
    waitingForResponse = false;
    // Restore selection display
    lcd->printLine(0, "Quiz " + String(quizId));
    lcd->printLine(1, "Select: " + String(currentSelection));
  }
}

void QuizModule::update() {
  if (!active || waitingForResponse)
    return;

  Button btn = InputManager::readButton();
  if (btn != BTN_NONE) {
    bool changed = false;
    if (btn == BTN_UP) {
      currentSelection++;
      changed = true;
    } else if (btn == BTN_DOWN) {
      currentSelection--;
      if (currentSelection < 0)
        currentSelection = 0;
      changed = true;
    } else if (btn == BTN_SELECT) {
      uart->sendResponse("GUESS:" + String(quizId) + ":" +
                         String(currentSelection));
      lcd->printLine(0, "Checking...");
      lcd->printLine(1, "Wait...");
      waitingForResponse = true;
      delay(500); // Prevent double press
      return;
    }

    if (changed) {
      lcd->printLine(1, "Select: " + String(currentSelection) + "   ");
      uart->sendResponse("GUESS:" + String(quizId) + ":" +
                         String(currentSelection));
      delay(200); // Simple debounce
    }
  }
}

bool QuizModule::isActive() { return active; }

void QuizModule::stop() { active = false; }
