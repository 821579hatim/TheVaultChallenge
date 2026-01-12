#ifndef UART_MANAGER_HPP
#define UART_MANAGER_HPP

#include <Arduino.h>

#include "uart_buffer.hpp"
#include <Arduino.h>

class UARTManager {
public:
  void begin(unsigned long baud);
  bool available();
  String readCommand();
  void sendResponse(const String &response);
  void sendLog(const String &message);
};

#endif
