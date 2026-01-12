# The Vault Challenge - Final Project Report

## 1. Project Overview & Assumptions

**Goal**: The objective of this project was to design and implement "The Vault Challenge," an interactive arcade-style system that bridges the gap between embedded hardware control and modern web-based visualization. The project simulates a high-stakes "heist" scenario where the user acts as a hacker trying to break into a physical vault by completing a series of digital and physical challenges.

**System Architecture**: The project adopts a hybrid architecture:
*   **Hardware (Embedded)**: An Arduino Uno handles real-time inputs (reflexes, memory sequences) and physical actuation (Servo motor for the vault).
*   **Software (PC)**: A Python Flask web server provides a cinematic "Arcade Mode" interface, rendering game visuals, instructions, and quizzes on a computer screen.
*   **Communication**: The two systems are synchronized via a custom Serial (UART) protocol, ensuring low-latency mirroring of actions.

**Key Features**:
1.  **Reflex Test (Level 1)**: Measures user reaction time in milliseconds using hardware interrupts.
2.  **Memory Challenge (Level 2)**: Implements a "Simon Says" pattern matching game using the LCD Keypad.
3.  **Cyber Runner (Level 3)**: A browser-based endless runner game controlled physically by the Arduino's Up/Down buttons.
4.  **Educational Quizzes**: "Security Checks" between levels require the user to answer technical questions about the Arduino platform (e.g., voltage levels, pin counts).
5.  **Physical Vault**: Upon successful completion, a Servo motor physically rotates to unlock a container.

---

## 2. Hardware Design & Schematic

The system is built around the **Arduino Uno (ATmega328P)**. It utilizes an **LCD Keypad Shield** for input and display, keeping the wiring compact and reliable.

### 2.1 Connection Diagram (Schematic)

*(Please insert your SimulIDE or Wokwi screenshot here)*

**[INSERT SCHEMATIC IMAGE HERE]**

### 2.2 Pinout Configuration

| Component | Arduino Pin | Type | Logic/Notes |
| :--- | :--- | :--- | :--- |
| **LCD Display (16x2)** | SDA / SCL | I2C | Address 0x27. Used for local status feedback. |
| **D-Pad Buttons** | A0 | Analog | Resistor Ladder Network (Voltage Divider). |
| **Servo Motor** | D6 | PWM | Controls the vault locking mechanism. |
| **PC Interface** | USB Port | Serial | UART Communication at 9600 baud. |

### 2.3 Input Logic (Resistor Ladder)
Instead of using 5 digital pins for 5 buttons, the system uses a single Analog Pin (**A0**). Each button connects to a voltage divider, producing a unique analog voltage when pressed:
*   **UP**: ~144 (Analog Value)
*   **DOWN**: ~329 (Analog Value)
*   **LEFT**: ~505 (Analog Value)
*   **RIGHT**: ~0 (Analog Value)
*   **SELECT**: ~741 (Analog Value)

The `InputManager` class reads `A0` and maps these values to `BTN_UP`, `BTN_DOWN`, etc., handling debouncing in software.

---

## 3. Implementation Details

The implementation is divided into two distinct repositories: **Firmware (C++)** and **Application (Python/JS)**.

### 3.1 Firmware Implementation (PlatformIO)
The firmware runs on the Arduino and is written in C++ using an Object-Oriented approach for modularity.

*   **Finite State Machine (FSM)**: The main loop (`main.cpp`) acts as a state manager, transitioning between `IDLE`, `RUN_GAME1`, `RUN_GAME2`, etc., based on commands received from the PC.
*   **Managers**:
    *   **`LCDManager`**: Abstraction layer for the I2C LCD, simplifying text rendering.
    *   **`UARTManager`**: Handles non-blocking serial communication, parsing incoming commands like `START_QUIZ:1` and sending telemetry like `GAME1_OK:350`.
*   **Game Logic**:
    *   **`Game1_Reflex`**: Uses `millis()` to track reaction time with millisecond precision.
    *   **`Game2_Memory`**: Generates pseudo-random sequences and verifies user input step-by-step.

### 3.2 Web Application (Python Flask)
The PC-side application provides the "Arcade" experience.
*   **Backend (`app.py`)**: A Flask server running a background thread that constantly reads from the Serial Port. It forwards data to the frontend using **SocketIO** events (`serial_data`).
*   **Frontend (HTML/JS/CSS)**:
    *   **Cinematics**: A "Retro Hacker" aesthetic using CSS animations (CRT scanlines, neon glows).
    *   **Mirroring**: JavaScript listens for `serial_data` events. If the Arduino sends `G1_TARGET:UP`, the web page instantly highlights the "UP" arrow.
    *   **Game 3**: A customized HTML5 Canvas game where the "Jump" and "Duck" actions are triggered not by keyboard, but by `socket.on` events received from the Arduino buttons.

---

## 4. Operation & Results

### 4.1 Usage Flow
1.  **Start**: The user launches the Python app and clicks "INITIALIZE HACK".
2.  **Gameplay**: The system guides the user through Levels 1, 2, and 3. Instructions appear on the big screen, while physical actions happen on the Arduino.
3.  **Quizzes**: Between levels, the system pauses for a "Security Check". The user inputs numerical answers using the Arduino D-Pad.
4.  **Victory**: Upon entering the final code (5-6-8), the Servo rotates (`vault.unlock()`), and the screen displays a victory animation with gold coins.

### 4.2 Screenshots of Operation

**[INSERT SCREENSHOT OF WEB INTERFACE (GAME SCREEN) HERE]**
*(Caption: The "Cyber Runner" game controlled by Arduino inputs)*

**[INSERT SCREENSHOT OF ARDUINO SETUP HERE]**
*(Caption: The physical setup showing the LCD and Buttons)*

---

## 5. Summary

"The Vault Challenge" successfully demonstrates the integration of low-level hardware control with high-level software application. By offloading time-critical tasks (reaction measurement) to the Arduino and rich visual tasks to the PC, the system leverages the strengths of both platforms. The project fulfills all functional requirements, including robust serial communication, state machine logic, and a polished user interface.
