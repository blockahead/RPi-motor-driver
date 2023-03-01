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

### Data write register (80bit)

| Bit | Description |
| - | - |
| DW79-DW72 | 0x78 (start flag) |
|||
| DW71-DW68 | ADDR2 |
| DW67-DW64 | ADDR1 |
|||
| DW63-DW32 | WDATA2 |
|||
| DW31-DW0 | WDATA1 |

### Data read register (80bit)

| Bit | Description |
| - | - |
| DR79-DR64 | CPU time (ms) |
|||
| DR63-DR32 | RDATA2 |
|||
| DR31-DR0 | RDATA1 |

### Data address

| ADDR3 | ADDR2 | ADDR1 | ADDR0 | Description |
| - | - | - | - | - |
| 0 | 0 | 0 | 0 | Do not care |
| 0 | 0 | 0 | 1 | Motor current (read only) |
| 0 | 0 | 1 | 0 | Motor speed (read only) |
| 0 | 0 | 1 | 1 | Motor position (read only) |
| 0 | 1 | 0 | 0 | Control mode |
| 0 | 1 | 0 | 1 | Control target |
| 0 | 1 | 1 | 0 | Motor supply voltage |
| 0 | 1 | 1 | 1 | Reserved |
| 1 | 0 | 0 | 0 | Current feedback parameter Kp |
| 1 | 0 | 0 | 1 | Current feedback parameter Ti |
| 1 | 0 | 1 | 0 | Speed feedback parameter Kp |
| 1 | 0 | 1 | 1 | Speed feedback parameter Ti |
| 1 | 1 | 0 | 0 | Position feedback parameter Kp |
| 1 | 1 | 0 | 1 | Position feedback parameter Ti |
| 1 | 1 | 1 | 0 | Position feedback parameter Td |
| 1 | 1 | 1 | 1 | Transmit data address |

### Data (WDATA, RDATA)

#### Motor current (read only)

| Bit | Description |
| - | - |
| DATA31-DATA0 | Motor current (A) as 32bit float |

#### Motor speed (read only)

| Bit | Description |
| - | - |
| DATA31-DATA0 | Motor speed (rad/s) as 32bit float |

#### Motor position (read only)

| Bit | Description |
| - | - |
| DATA31-DATA0 | Motor position (rad) as 32bit float |

#### Control mode

| Bit | Description |
| - | - |
| DATA31-DATA3 | 0 |
| DATA2-DATA0 | Mode select |

| DATA2 | DATA1 | DATA0 | Description |
| - | - | - | - |
| 0 | 0 | 0 | Disable |
| 0 | 0 | 1 | Voltage control mode (no feedback) |
| 0 | 1 | 0 | Current (torque) control mode (PI) |
| 0 | 1 | 1 | Speed control mode (PI) |
| 1 | 0 | 0 | Position control mode (PID) |
| 1 | 0 | 1 | Reserved |
| 1 | 1 | 0 | Reserved |
| 1 | 1 | 1 | Reserved |

#### Control target

| Bit | Description |
| - | - |
| DATA31-DATA0 | Target value as 32bit float |

#### Motor supply voltage

| Bit | Description |
| - | - |
| DATA31-DATA0 | Supply voltage (V) as 32bit float |

#### Feedback parameters

ADDR = 0b1000 - 0b1110
| Bit | Description |
| - | - |
| DATA31-DATA0 | Feedback parameter as 32bit float |

#### Transmit data address

| Bit | Description |
| - | - |
| DATA31-DATA4 | 0 |
| DATA3-DATA0 | Transmit [data address](#data-address) |

## Design sheets

+ [Controller](docs/Controller.md)
+ [Motor driver](docs/MotorDriver.md)
+ [Current sense amplifier](docs/CurrentSenseAmp.md)
