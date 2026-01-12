#ifndef VAULT_HPP
#define VAULT_HPP

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>

class Vault {
private:
  Servo servo;
  uint8_t servoPin;
  String currentPin;
  bool isLocked;
  int failedAttempts;
  unsigned long blockEndTime;

public:
  Vault(uint8_t pin);
  void begin();
  void open();
  void close();
  bool unlock(String pin);
  void setPin(String newPin);
  String getPin(); // For debugging/admin
  bool isBlocked();
  void checkBlock();
  void resetFailedAttempts();
};

#endif
