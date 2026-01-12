# The Vault Challenge

## Overview
Academic project using Arduino Uno and a Python Mac App to create a "Vault" unlocking game.

## Hardware
- Arduino Uno
- LCD Keypad Shield (16x2 LCD + 5 Buttons on A0)
- Servo (D6)
- Buzzer (D8)
- Potentiometer (A1)
- External Button (D2)

## Software
- **Firmware**: PlatformIO (C++)
- **App**: Python 3 (Tkinter + pyserial)

## How to Run

### 1. Firmware
1.  Install PlatformIO (VSCode Extension).
2.  Open `TheVaultChallenge` folder.
3.  Run `pio run -t upload` to flash Arduino.

### 2. Simulation (Wokwi)
1.  Go to [Wokwi](https://wokwi.com).
2.  Create new Arduino Uno project.
3.  Copy `src/main.cpp` content (and all other files flattened or use library tab).
4.  Copy `simulation/wokwi/diagram.json` to diagram tab.

### 3. Python App
1.  Install dependencies: `pip install -r python_app/requirements.txt`
2.  Run: `python python_app/main.py`
3.  Select the Arduino Serial Port and Connect.

## Architecture
- **Modular Design**: Separate classes for LCD, ADC, UART, and Games.
- **Mock Libraries**: `HD44780`, `libADC`, `uart_buffer` are mocked to match Professor's API.
