#include "UARTManager.hpp"
#include <Arduino.h>

void UARTManager::begin(unsigned long baud) {
  uart_init(baud, 0); // 0 = no high speed
}

bool UARTManager::available() { return uart_read_count() > 0; }

String UARTManager::readCommand() {
  String line = "";
  // Check if we have a full line?
  // The original code used a buffer. Here we must poll.
  // This function is called in loop. We should probably buffer characters until
  // \n. But since we can't add state easily without changing header, let's
  // To properly implement readLine without blocking, we need a static buffer or
  // member buffer. I'll use a static buffer here for simplicity, or just read
  // one char at a time in loop? The previous implementation assumed
  // `uart.readLine` did the buffering. I need to implement buffering here.

  static String inputBuffer = "";

  while (uart_read_count() > 0) {
    char c = (char)uart_read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        String cmd = inputBuffer;
        inputBuffer = "";
        return cmd;
      }
    } else {
      inputBuffer += c;
    }
  }
  return "";
}

void UARTManager::sendResponse(const String &response) {
  uart_send_string((uint8_t *)response.c_str());
  uart_send_byte('\n');
}

void UARTManager::sendLog(const String &message) {
  String logMsg = "LOG:" + message;
  sendResponse(logMsg);
}
