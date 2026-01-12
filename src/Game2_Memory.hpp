#ifndef GAME2_MEMORY_HPP
#define GAME2_MEMORY_HPP

#include "ADCManager.hpp"
#include "LCDManager.hpp"
#include "TimerUtils.hpp"
#include "UARTManager.hpp"
#include <Arduino.h>

class Game2_Memory {
private:
  LCDManager *lcd;
  UARTManager *uart;
  bool active;
  String targetSequence; // e.g., "UDLR"
  int currentStep;
  bool waitingForInput;
  unsigned long lastStepTime;
  int inputIndex;
  bool showingStep; // True if currently showing a direction on LCD

  void displayDirection(char dir);

public:
  Game2_Memory(LCDManager *l, UARTManager *u);
  void start(String sequence);
  void update();
  bool isActive();
  void stop();
};

#endif
