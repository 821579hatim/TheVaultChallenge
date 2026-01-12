# SimulIDE Instructions

1.  Open SimulIDE.
2.  Drag **Arduino Uno** from the component list.
3.  Drag **LCD 16x2** (HD44780).
4.  Connect LCD pins:
    *   RS -> D8
    *   EN -> D9
    *   D4 -> D4
    *   D5 -> D5
    *   D6 -> D6
    *   D7 -> D7
    *   RW -> GND
5.  Drag **Servo** and connect to **D6**.
6.  Drag **Buzzer** (Active) and connect to **D8** (and GND).
7.  Drag **Potentiometer** for Game 3:
    *   Middle pin -> **A1**
    *   Side pins -> 5V and GND
8.  Drag **Push Button** for Game 2:
    *   One side -> **D2**
    *   Other side -> GND
    *   (Enable Internal Pullup in code handles the rest)
9.  **Keypad Shield Simulation**:
    *   The easiest way is to use a **Potentiometer** connected to **A0**.
    *   Varying the voltage simulates different button presses (Right=0V, Up=0.7V, etc).
10. **Serial Communication**:
    *   Right click Arduino -> Properties -> Open Serial Monitor.
    *   Or use a "Serial Port" component connected to RX/TX to bridge to a real virtual COM port if testing with the Python app.
