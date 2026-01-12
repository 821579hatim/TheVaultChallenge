#ifndef LCD_MANAGER_HPP
#define LCD_MANAGER_HPP

#include <Arduino.h>

#include "HD44780.hpp"
#include <Arduino.h>

// Wrapper for LCD operations
class LCDManager {
public:
  LCDManager();
  void begin();
  void clear();
  void printLine(uint8_t row, const String &text);
  void print(const String &text);
  void printStatus(const String &status);
};

#endif
