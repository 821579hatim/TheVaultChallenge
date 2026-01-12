# PROJECT REPORT: THE VAULT CHALLENGE
**Subject:** Microcontrollers / Embedded Systems
**Date:** January 2026

---

## 1. Title Page
*(Space reserved for student details. Please fill in the following fields)*
*   **Project Title:** The Vault Challenge: Hybrid Arcade Entertainment System
*   **Author:** [Your Full Name]
*   **Degree:** [Your Degree/Major]
*   **Group:** [Your Practical Group]
*   **Call:** January 2026

---

## 2. Abstract
"The Vault Challenge" is an engineering project that integrates embedded systems and modern web development to create a unique gamified user experience. The system consists of a physical vault controlled by an Arduino Uno microcontroller, which interacts in real-time with an "Arcade" graphical interface deployed on a PC. The main objective is to demonstrate the viability of hybrid architectures where hardware manages mission-critical real-time tasks (such as millisecond-precision reflex measurement) while high-level software handles business logic and visual presentation. The project implements robust serial communication protocols, finite state machines, and modular software architecture, resulting in a polished, educational, and entertaining final product.

---

## 3. Introduction and Objectives
In the current context of the Internet of Things (IoT) and Industry 4.0, the interaction between the physical and digital worlds is fundamental. This project was born with the aim of exploring and mastering this interface.

**Main Objectives:**
1.  **Hardware-Software Synergy:** To design a system where the Arduino is not merely a passive peripheral, but an active controller that conditions the execution of a complex web application.
2.  **Educational Gamification:** To convert a technical assessment (questions about voltage, pins, bits) into a game mechanic ("Security Checks") to make learning more engaging.
3.  **Technical Robustness:** To implement professional design patterns (State Machines, Software Debouncing, Asynchronous Communication) to ensure fail-safe operation.

---

## 4. Hardware and System Design
The core of the hardware is an **Arduino Uno (ATmega328P)**. The design has been optimized to maximize functionality using the minimum number of pins, employing analog multiplexing techniques.

### 4.1 Wiring Scheme
*(Please insert your SimulIDE or Wokwi screenshot here)*

**[INSERT SCHEMATIC IMAGE HERE]**

*   **Human Machine Interface (HMI):** A **16x2 LCD** screen connected via **I2C** bus (SDA/SCL lines) is used to reduce wiring from 6 to 2 wires.
*   **Actuator:** A **Servo Motor** connected to digital pin **D6** (PWM) simulates the vault locking mechanism.
*   **Communication:** The Arduino's USB port acts as a virtual serial port (UART) at 9600 baud.

### 4.2 Analog Keypad (Resistor Ladder)
One of the design challenges was managing 5 buttons (Up, Down, Left, Right, Select) with a limited number of pins.
**Solution:** Instead of using 5 digital pins, a **Resistor Ladder** connected to a single analog pin (**A0**) was implemented.
*   **Operation:** Each button closes the circuit through a resistor of a different value, creating a unique voltage divider.
*   **Reading:** The Arduino's ADC (Analog-to-Digital Converter) reads a voltage value (0-1023). The software (`InputManager`) decodes this value into ranges (e.g., 0-50 = Right, 140-150 = Up) to identify the pressed button. This allows controlling a full interface with a single I/O pin.

---

## 5. Software Implementation

### 5.1 Firmware (C++ / PlatformIO)
The Arduino code follows an **Object-Oriented Programming** paradigm, modularizing each hardware component into a class (`LCDManager`, `UARTManager`, `Vault`).
The core logic resides in the `main.cpp` file, which implements a **Finite State Machine (FSM)**.
*   **States:** `IDLE` (Waiting for command) ⮕ `RUN_GAME1` (Reflexes) ⮕ `RUN_GAME2` (Memory) ⮕ `RUN_QUIZ` (Questions) ⮕ `WAIT_PIN` (Opening).
*   **Advantage:** This architecture prevents race conditions and ensures the system only responds to permitted commands during each game phase.

### 5.2 Backend (Python Flask)
The web server acts as a bridge between the browser and the Arduino.
*   **Multithreading:** Flask executes a background thread (`background_thread`) that continuously polls the serial port. This is critical: if the reading were synchronous, the web interface would freeze waiting for data from the Arduino.
*   **SocketIO:** Uses WebSockets to "push" data to the web in real-time, allowing the interface to react instantly (e.g., lighting up an arrow) when a physical button is pressed.

### 5.3 Communication Protocol
A custom ASCII text protocol was designed to facilitate debugging:
*   **PC ⮕ Arduino:** Imperative Commands (e.g., `START_GAME1`, `UNLOCK:568`).
*   **Arduino ⮕ PC:** Telemetry and Events (e.g., `GAME1_OK:350` to report 350ms reflex time, `GUESS:1:5` to report the user selected answer 5).

---

## 6. User Manual
1.  **Connection:** Connect the Arduino Uno to the computer's USB port.
2.  **Startup:** Run the `run_app.py` script. The browser will open automatically.
3.  **Configuration:** On the "Attract Mode" screen, select the correct COM port and press "CONNECT SYSTEM".
4.  **Gameplay:**
    *   Press "INITIALIZE HACK".
    *   **Level 1:** Follow the voice/text instructions. Press the indicated button as fast as possible.
    *   **Security Check:** Use Up/Down to select the correct answer.
    *   **Level 2:** Repeat the displayed direction sequence.
    *   **Level 3:** Use the Arduino buttons to control the on-screen character.
5.  **Finale:** Enter the obtained code. The servo will open.

### Screenshots of Operation

**[INSERT SCREENSHOT OF WEB INTERFACE (GAME SCREEN) HERE]**
*(Caption: The "Cyber Runner" game controlled by Arduino inputs)*

**[INSERT PHOTO OF ARDUINO SETUP HERE]**
*(Caption: The physical setup showing the LCD and Buttons)*

---

## 7. Conclusions
The development of "The Vault Challenge" has consolidated key knowledge in embedded system design:
1.  **Resource Management:** The use of I2C and multiplexed analog inputs demonstrates how to optimize pin usage on limited microcontrollers.
2.  **Asynchronous Communication:** The main technical difficulty (synchronizing the web with hardware) was effectively resolved using an event-based design and WebSockets, eliminating perceptible latency.
3.  **User Experience (UI/UX):** It has been demonstrated that a polished "retro" interface (CSS/JS) significantly increases the perceived value of a simple hardware project.

This project lays the foundation for future, more complex IoT developments, validating the Arduino-Python-Web architecture as a robust and scalable solution.
