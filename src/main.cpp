#include "ADCManager.hpp"
#include "ControllerMode.hpp"
#include "Game1_Reflex.hpp"
#include "Game2_Memory.hpp"
#include "Interrupts.hpp"
#include "LCDManager.hpp"
#include "QuizModule.hpp"
#include "TimerUtils.hpp"
#include "UARTManager.hpp"
#include "Vault.hpp"
#include <Arduino.h>

// Global Managers
LCDManager lcd;
UARTManager uart;
Vault vault(6); // Servo on D6

// Games & Modules
Game1_Reflex game1(&lcd, &uart);
Game2_Memory game2(&lcd, &uart);
ControllerMode game3(&lcd, &uart); // Cyber Runner Controller
QuizModule quiz(&lcd, &uart);

enum State { IDLE, RUN_GAME1, RUN_GAME2, RUN_GAME3, RUN_QUIZ, WAIT_PIN };

State currentState = IDLE;
String inputBuffer = "";

void setup() {
  // Init Hardware
  InputManager::init();
  TimerUtils::init();
  Interrupts::init();
  uart.begin(9600);
  lcd.begin();
  vault.begin();
  vault.setPin("568"); // Force correct PIN for Arcade Mode

  lcd.printLine(0, "THE VAULT");
  lcd.printLine(1, "INSERT COIN");

  uart.sendLog("BOOT:OK");
}

void processCommand(String cmd) {
  uart.sendLog("CMD_RCV:" + cmd);

  // --- GAME 1 ---
  if (cmd == "START_GAME1") {
    game1.start();
    currentState = RUN_GAME1;
  }

  // --- GAME 2 ---
  else if (cmd.startsWith("START_GAME2")) {
    int sep = cmd.indexOf(':');
    String seq = (sep > 0) ? cmd.substring(sep + 1) : "UDLR";
    game2.start(seq);
    currentState = RUN_GAME2;
  }

  // --- GAME 3 (CYBER RUNNER) ---
  else if (cmd == "START_GAME3") {
    game3.start();
    currentState = RUN_GAME3;
  }

  // --- QUIZ SYSTEM ---
  else if (cmd.startsWith("START_QUIZ:")) {
    int id = cmd.substring(11).toInt();
    quiz.start(id);
    currentState = RUN_QUIZ;
  } else if (cmd.startsWith("QUIZ_RESULT:")) {
    if (currentState == RUN_QUIZ) {
      String res = cmd.substring(12); // CORRECT or WRONG
      quiz.handleResponse(res);
    }
  }

  // --- VAULT ---
  else if (cmd.startsWith("UNLOCK:")) {
    String pin = cmd.substring(7);
    if (vault.unlock(pin)) {
      uart.sendLog("STATUS:UNLOCKED");
      lcd.printLine(0, "ACCESS GRANTED");
      lcd.printLine(1, "VAULT OPEN");
    } else {
      uart.sendLog("STATUS:DENIED");
      lcd.printLine(0, "ACCESS DENIED");
      lcd.printLine(1, "BAD PIN");
    }
  } else if (cmd == "RESET") {
    game1.stop();
    game2.stop();
    game3.stop();
    quiz.stop();
    vault.close();
    lcd.clear();
    lcd.printLine(0, "THE VAULT");
    lcd.printLine(1, "INSERT COIN");
    currentState = IDLE;
  }
}

void loop() {
  if (vault.isBlocked()) {
    lcd.printLine(0, "SYSTEM BLOCKED");
    lcd.printLine(1, "Wait...");
    vault.checkBlock(); // Will clear when time is up
    return;
  }

  // UART Handling
  if (uart.available()) {
    String cmd = uart.readCommand();
    if (cmd.length() > 0) {
      processCommand(cmd);
    }
  }

  // State Machine
  switch (currentState) {
  case IDLE:
    // Do nothing, wait for command
    break;
  case RUN_GAME1:
    game1.update();
    if (!game1.isActive())
      currentState = IDLE;
    break;
  case RUN_GAME2:
    game2.update();
    if (!game2.isActive())
      currentState = IDLE;
    break;
  case RUN_GAME3:
    game3.update();
    if (!game3.isActive())
      currentState = IDLE;
    break;
  case RUN_QUIZ:
    quiz.update();
    if (!quiz.isActive())
      currentState = IDLE;
    break;
  case WAIT_PIN:
    // Manual PIN entry on Arduino (Optional feature)
    break;
  }
}
