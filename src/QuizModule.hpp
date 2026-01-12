#ifndef QUIZ_MODULE_HPP
#define QUIZ_MODULE_HPP

#include <Arduino.h>

#include "ADCManager.hpp"
#include "LCDManager.hpp"
#include "UARTManager.hpp"
#include <Arduino.h>

class QuizModule {
private:
  LCDManager *lcd;
  UARTManager *uart;
  bool active;
  int currentSelection;
  bool waitingForResponse;
  int quizId; // To track which quiz we are on

public:
  QuizModule(LCDManager *l, UARTManager *u);
  void start(int id);
  void update();
  void handleResponse(String response); // Called when Mac sends CORRECT/WRONG
  bool isActive();
  void stop();
};

#endif
