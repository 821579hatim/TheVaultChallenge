# The Vault Challenge - Project Report

## 1. Introduction
This project implements a secure vault system controlled by an Arduino Uno and a companion Mac application. The user must complete three minigames to unlock the vault.

## 2. System Architecture
The system consists of two main components:
*   **Arduino Firmware**: Handles hardware control (Servo, LCD, Sensors) and executes real-time game logic.
*   **Python Application**: Provides a GUI for game selection, status monitoring, and administration.

### 2.1 Hardware
*   **Arduino Uno**: Central controller.
*   **LCD Keypad Shield**: User interface (Display + Navigation).
*   **Servo Motor**: Locking mechanism.
*   **Buzzer**: Audio feedback for Morse code.
*   **Sensors**: Potentiometer (Analog Game) and Button (Reflex/Morse).

## 3. Implementation Details
### 3.1 Firmware (C++)
Developed using PlatformIO. Key modules:
*   `main.cpp`: State machine managing system states (IDLE, GAMES, LOCKED).
*   `Managers`: `LCDManager`, `ADCManager`, `UARTManager` abstract hardware.
*   `Games`: Encapsulated logic for Reflex, Morse, and Analog games.
*   **Libraries**: Custom mocks were created for `HD44780`, `libADC`, and `uart_buffer` to simulate the academic environment.

### 3.2 Software (Python)
*   **GUI**: Tkinter-based interface.
*   **Communication**: `pyserial` handles UART commands with a robust protocol (ACK/NACK implicit via state updates).
*   **Threading**: Serial I/O runs in a separate thread to keep GUI responsive.

## 4. Simulation
The project was verified using:
*   **Wokwi**: For logic and wiring verification.
*   **SimulIDE**: For electrical schematic validation.

## 5. Conclusion
The project successfully integrates hardware and software to create an interactive system. The modular design allows for easy extension (e.g., adding more games or sensors).
