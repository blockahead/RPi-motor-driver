# RPi-motor-driver

## Overview

+ DC Motor driver board for Raspberry Pi
  + Connected with a Raspberry Pi through the Serial Peripheral Interface (SPI)
  + Locked anti phase (LAP) PWM
  + Contollable by voltage / current (torque)
  + only Raspberry Pi 4B tested
+ CPU and peripherals
  + F3 series CPU (STM32F303K8T6; STMicroelectronics)
  + Current sense amplifier (MAX4071; Maxim Integrated)
  + Full bridge FET driver (HIP4080A; Renesas Electronics)
+ Specs
  + Motor control
    + Locked anti phase PWM
    + Supports up to 12V/3A
    + 40KHz PWM
    + 0.5% resolution (from -100% to +100% of supply voltage)
  + Rotation speed measurement
    + Open collector incremental rotary encoder (not support Z phase)
  + Current measurement
    + Bidirectional (-3A to +3A)
    + 50us sampling period

## SPI protocol

### Request register

| Bit | Description |
| - | - |
| RQ7 | 0 (ready) |
| RQ6 | 1 (start bit) |
| RQ5 | 0: read only, 1: write |
| RQ4 | 0: CH1, 1: CH2 |
| RQ3-RQ0 | Data address |

| RQ3 | RQ2 | RQ1 | RQ0 | Description |
| - | - | - | - | - |
| 0 | 0 | 0 | 0 | Motor current (read only) |
| 0 | 0 | 0 | 1 | Motor speed (read only) |
| 0 | 0 | 1 | 0 | Motor position (read only) |
| 0 | 0 | 1 | 1 | Reserved |
| 0 | 1 | 0 | 0 | Control mode |
| 0 | 1 | 0 | 1 | Control target |
| 0 | 1 | 1 | 0 | Motor supply voltage |
| 0 | 1 | 1 | 1 | Reserved |
| 1 | 0 | 0 | 0 | Current feedback gain Kp |
| 1 | 0 | 0 | 1 | Current feedback gain Ti |
| 1 | 0 | 1 | 0 | Speed feedback gain Kp |
| 1 | 0 | 1 | 1 | Speed feedback gain Ti |
| 1 | 1 | 0 | 0 | Position feedback gain Kp |
| 1 | 1 | 0 | 1 | Position feedback gain Ti |
| 1 | 1 | 1 | 1 | Position feedback gain Td |

### Data write register

#### Read only (RQ3-RQ0 = 0b00XX)

| Bit | Description |
| - | - |
| DW31-DW0 | 0 (do not care) |

#### Control mode (RQ3-RQ0 = 0b0100)

| Bit | Description |
| - | - |
| DW31-DW3 | 0 |
| DW2-DW0 | Mode select |

| DW2 | DW1 | DW0 | Description |
| - | - | - | - |
| 0 | 0 | 0 | Disable |
| 0 | 0 | 1 | Voltage control mode|
| 0 | 1 | 0 | Current (torque) control mode (PI) |
| 0 | 1 | 1 | Speed control mode (PI) |
| 1 | 0 | 0 | Position control mode (PID) |
| 1 | 0 | 1 | Reserved |
| 1 | 1 | 0 | Reserved |
| 1 | 1 | 1 | Reserved |

#### Control target (RQ3-RQ0 = 0b0101)

| Bit | Description |
| - | - |
| DW31-DW0 | Target value as 32bit float |

#### Motor supply voltage (RQ3-RQ0 = 0b0110)

| Bit | Description |
| - | - |
| DW31-DW0 | Supply voltage (V) as 32bit float |

#### Motor feedback gain (RQ3-RQ0 = 0b1XXX)

| Bit | Description |
| - | - |
| DW31-DW0 | Feedback gain as 32bit float |

### Data read register

#### Motor current (RQ3-RQ0 = 0b0000)

| Bit | Description |
| - | - |
| DW31-DW0 | Motor current (A) as 32bit float |

#### Motor speed (RQ3-RQ0 = 0b0001)

| Bit | Description |
| - | - |
| DW31-DW0 | Motor speed (rad/s) as 32bit float |

#### Motor position (RQ3-RQ0 = 0b0010)

| Bit | Description |
| - | - |
| DW31-DW0 | Motor position (rad) as 32bit float |

#### Other addresses

Same as the write register.

## Design sheets

+ [Controller](docs/Controller.md)
+ [Motor driver](docs/MotorDriver.md)
+ [Current sense amplifier](docs/CurrentSenseAmp.md)
