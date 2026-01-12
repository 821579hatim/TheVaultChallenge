# System Schematic & Pinout

## Connection Diagram
This diagram represents the connections between the Arduino Uno and its components.

```mermaid
graph TD
    subgraph Arduino Uno
        Processor[ATmega328P]
        USB[USB Serial to PC]
        A0[Pin A0 (Analog)]
        D6[Pin D6 (PWM)]
        I2C[I2C Bus (SDA/SCL)]
    end

    subgraph Peripherals
        Buttons[D-Pad Buttons]
        Servo[Servo Motor]
        LCD[LCD Display 16x2]
    end

    Processor <--> USB
    
    %% Input (Resistor Ladder)
    Buttons -- Voltage Divide Network --> A0
    
    %% Output
    D6 --> Servo
    
    %% Display
    I2C --> LCD
```

## Pinout Table

| Component | Arduino Pin | Type | Notes |
| :--- | :--- | :--- | :--- |
| **LCD Display** | SDA / SCL | I2C | Address usually 0x27 |
| **Buttons (All)** | A0 | Analog | Resistor Ladder Network |
| **Servo Motor** | D6 | PWM | Orange Wire (Signal) |
| **PC Connection** | USB Port | Serial | Baudraute 9600 |

## Note on Buttons (Keypad Shield)
This project uses a standard **LCD Keypad Shield** logic where multiple buttons are connected to a single Analog Pin (**A0**) using a voltage divider (resistor ladder).
*   **UP**: ~144 (Analog Value)
*   **DOWN**: ~329 (Analog Value)
*   **LEFT**: ~505 (Analog Value)
*   **RIGHT**: ~0 (Analog Value)
*   **SELECT**: ~741 (Analog Value)
