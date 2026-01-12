#include "ADCManager.hpp"
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

void InputManager::init() { ADC_Init(); }

// Helper to read specific channel using libADC + manual MUX
uint16_t readChannel(uint8_t channel) {
  // ADMUX: REFS0=1 (AVCC), MUX3:0 = channel
  ADMUX = (1 << REFS0) | (channel & 0x07);
  _delay_us(10); // Wait for MUX to stabilize
  return ADC_conversion();
}

Button InputManager::readButton() {
  int val = readChannel(0); // A0 is buttons

  // Thresholds for LCD Keypad Shield
  if (val > 1000)
    return BTN_NONE;
  if (val < 50)
    return BTN_RIGHT;
  if (val < 195)
    return BTN_UP;
  if (val < 380)
    return BTN_DOWN;
  if (val < 555)
    return BTN_LEFT;
  if (val < 790)
    return BTN_SELECT;

  return BTN_NONE;
}

int InputManager::readPotentiometer() {
  // A1 is optional potentiometer
  return readChannel(1);
}
