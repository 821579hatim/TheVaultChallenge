#include "Vault.hpp"

#define EEPROM_ADDR_PIN 0
#define PIN_LENGTH 3
#define BLOCK_DURATION 30000 // 30 seconds

Vault::Vault(uint8_t pin)
    : servoPin(pin), isLocked(true), failedAttempts(0), blockEndTime(0) {}

void Vault::begin() {
  servo.attach(servoPin);
  close(); // Start closed

  // Read PIN from EEPROM
  char storedPin[PIN_LENGTH + 1];
  for (int i = 0; i < PIN_LENGTH; i++) {
    storedPin[i] = EEPROM.read(EEPROM_ADDR_PIN + i);
  }
  storedPin[PIN_LENGTH] = '\0';

  // Check if valid (simple check)
  bool valid = true;
  for (int i = 0; i < PIN_LENGTH; i++) {
    if (!isdigit(storedPin[i]))
      valid = false;
  }

  if (valid) {
    currentPin = String(storedPin);
  } else {
    currentPin = "1234"; // Default
    setPin(currentPin);
  }
}

void Vault::open() {
  servo.write(90); // Open position
  isLocked = false;
}

void Vault::close() {
  servo.write(0); // Closed position
  isLocked = true;
}

bool Vault::unlock(String pin) {
  if (isBlocked())
    return false;

  if (pin == currentPin) {
    open();
    failedAttempts = 0;
    return true;
  } else {
    failedAttempts++;
    if (failedAttempts >= 3) {
      blockEndTime = millis() + BLOCK_DURATION;
    }
    return false;
  }
}

void Vault::setPin(String newPin) {
  if (newPin.length() != PIN_LENGTH)
    return;
  currentPin = newPin;
  for (int i = 0; i < PIN_LENGTH; i++) {
    EEPROM.write(EEPROM_ADDR_PIN + i, newPin[i]);
  }
}

String Vault::getPin() { return currentPin; }

bool Vault::isBlocked() {
  if (blockEndTime > 0) {
    if (millis() < blockEndTime) {
      return true;
    } else {
      blockEndTime = 0;
      failedAttempts = 0;
      return false;
    }
  }
  return false;
}

void Vault::checkBlock() {
  isBlocked(); // Updates state
}

void Vault::resetFailedAttempts() {
  failedAttempts = 0;
  blockEndTime = 0;
}
