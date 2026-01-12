#ifndef GAME1_REFLEX_HPP
#define GAME1_REFLEX_HPP

#include "ADCManager.hpp"
#include "LCDManager.hpp"
#include "TimerUtils.hpp"
#include "UARTManager.hpp"

class Game1_Reflex {
private:
  LCDManager *lcd;
  UARTManager *uart;
  unsigned long startTime;
  bool active;
  Button targetButton;
  unsigned long reactionLimit;

public:
  Game1_Reflex(LCDManager *l, UARTManager *u);
  void start();
  void update();
  bool isActive();
  void stop();
};

#endif
