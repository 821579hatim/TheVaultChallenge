# The Vault Challenge
## Academic Project Defense

---

# Overview
*   **Goal**: Unlock the Vault (Servo) by passing 3 challenges.
*   **Components**: Arduino Uno, LCD Shield, Python App.
*   **Communication**: UART (Serial) @ 9600 baud.

---

# Hardware Setup
*   **LCD Keypad Shield**: Interface.
*   **Servo (D6)**: Lock.
*   **Buzzer (D8)**: Sound.
*   **Potentiometer (A1)**: Analog Input.
*   **Button (D2)**: Interrupt Input.

---

# Minigames
1.  **Reflex**: Test reaction time (< 500ms).
2.  **Morse**: Replicate audio patterns.
3.  **Analog Match**: Precision potentiometer control.

---

# Software Architecture
*   **Arduino**: Modular C++ (Managers, Game Classes).
*   **Python**: Tkinter GUI + Threaded Serial.
*   **Protocol**: Text-based commands (`START_GAME`, `UNLOCK`).

---

# Demo
*   [Live Demo / Video Placeholder]
*   Showcasing:
    *   App Connection.
    *   Game Completion.
    *   Servo Unlocking.

---

# Conclusion
*   Robust bidirectional communication.
*   Real-time hardware control.
*   Modular and extensible code.
