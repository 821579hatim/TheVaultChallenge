# The Vault Challenge - Project Report

## 1. Project Assumptions
The goal of the project was to create an interactive "Arcade Vault" system that combines embedded hardware control with a modern web interface. The system mimics a cinematic "heist" scenario where a user must complete a series of mini-games and quizzes to unlock a physical vault.

### Key Features:
*   **Hybrid Architecture**: Interaction is split between a physical Arduino controller (reflexes, memory) and a PC Web Interface (logic, display).
*   **Gamified Flow**: Three distinct levels (Reflex, Memory, Cyber Runner) increasing in difficulty.
*   **Educational Aspect**: "Security Checks" (Quizzes) require knowledge of Arduino technical specifications.
*   **Real-time Synchronization**: The PC and Arduino communicate via Serial/UART to mirror actions (LEDs lighting up on screen, score updates).

## 2. System Schematic & Hardware
The system is built on an **Arduino Uno** with the following peripherals:
*   **Display**: I2C LCD 16x2 (Visual feedback on device).
*   **Input**: 5 Tactile Buttons (Up, Down, Left, Right, Select) arranged in a D-Pad configuration.
*   **Actuator**: Servo Motor (Simulating the vault locking mechanism).
*   **Communication**: USB Serial connection to PC.
*   **Visuals**: LCD Screen for text feedback.

*(Note for Report: Insert your Wokwi/SimulIDE screenshot here representing these connections)*

## 3. Implementation Discussion

### 3.1 Software Architecture (PlatformIO)
The firmware is developed in C++ using **Visual Studio Code** and **PlatformIO**. The code is structured efficiently using a modular **Object-Oriented approach**:

*   **`main.cpp`**: Implements a **Finite State Machine (FSM)**. The system cycles through states: `IDLE` -> `RUN_GAME1` -> `RUN_GAME2` -> `RUN_GAME3` -> `RUN_QUIZ` -> `WAIT_PIN`.
*   **Managers**:
    *   `UARTManager`: Handles non-blocking serial communication with the Python backend.
    *   `LCDManager`: Wraps the LiquidCrystal_I2C library for easy text rendering.
    *   `InputManager`: Handles button debouncing and analog reading.
*   **Game Modules**: Each mini-game (`Game1_Reflex`, `Game2_Memory`) is encapsulated in its own class, maintaining its own internal state and logic.

### 3.2 Web Application (Python & JavaScript)
The "Arcade Mode" is powered by a **Python Flask** server handling Serial communication on a background thread.
*   **Backend (`app.py`)**: Uses `flask_socketio` to bridge the Serial Port data to the Web Browser in real-time.
*   **Frontend**: A cinematic HTML/CSS/JS interface that mirrors the Arduino state.
    *   **Visual Mirroring**: When the Arduino selects a button, the web interface highlights the corresponding direction.
    *   **Game Logic**: Level 3 ("Cyber Runner") is a Canvas-based browser game controlled physically by the Arduino's buttons.

### 3.3 Communication Protocol
Custom text-based protocol for reliability:
*   **PC -> Arduino**: `START_GAME1`, `START_QUIZ:1`, `UNLOCK:568`, `RESET`.
*   **Arduino -> PC**: `GAME1_OK:350` (Reaction time), `GUESS:1:5` (Real-time quiz value), `STATUS:UNLOCKED`.

## 4. Operation Summary
The user flow operates as follows:
1.  **Initialization**: User connects Arduino and clicks "INITIALIZE HACK" on the web.
2.  **Level 1 (Reflex)**: PC shouts a direction; User presses the button on Arduino. Arduino measures reaction time.
3.  **Level 2 (Memory)**: Arduino flashes a LED sequence; User repeats it. Web mirrors the sequence.
4.  **Level 3 (Cyber Runner)**: User controls a web-based runner game using Arduino buttons (Jump/Duck).
5.  **The Vault**: After passing quizzes, the PC sends the `UNLOCK` command. The Arduino rotates the Servo, physically opening the vault.

## 5. Summary
The project successfully integrates embedded logical control with high-level web visualization. It demonstrates proficiency in C++ programming, Serial communication protocols, state machine design, and full-stack web development.
