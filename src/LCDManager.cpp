#include "LCDManager.hpp"

// Wrapper for Professor's HD44780 library
LCDManager::LCDManager() {
  // No constructor needed for C lib
}

void LCDManager::begin() {
  LCD_Initalize();
  LCD_Clear();
  LCD_WriteText((char *)"Vault Challenge");
}

void LCDManager::clear() { LCD_Clear(); }

void LCDManager::printLine(uint8_t row, const String &text) {
  LCD_GoTo(0, row);
  // Pad with spaces to clear line
  String padded = text;
  while (padded.length() < 16) {
    padded += " ";
  }
  LCD_WriteText((char *)padded.c_str());
}

void LCDManager::print(const String &text) {
  LCD_WriteText((char *)text.c_str());
}

void LCDManager::printStatus(const String &status) {
  printLine(0, "Status:");
  printLine(1, status);
}
