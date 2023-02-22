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
| DW79-DW64 | 0 |
|||
| DW63-DW56 | 0x78 (start flag) |
|||
| DW55 | 0: CH1, 1: CH2 |
| DW54-DW52 | 0 |
| DW51-DW48 | DTYPE (data read type of HRDATA) |
|||
| DW47 | 0: CH1, 1: CH2 |
| DW46-DW44 | 0 |
| DW43-DW40 | DTYPE (data read type of LRDATA) |
|||
| DW39 | 0: CH1, 1: CH2 |
| DW38-DW36 | 0 |
| DW35-DW32 | DTYPE (data write type of WDATA) |
|||
| DW31-DW0 | WDATA |

### Data read register (80bit)

| Bit | Description |
| - | - |
| DR79-DR64 | CPU time (ms) |
|||
| DR63-DR32 | HRDATA |
|||
| DR31-DR0 | LRDATA |

### Data type

| DTYPE3 | DTYPE2 | DTYPE1 | DTYPE0 | Description |
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
| 1 | 1 | 1 | 1 | Reserved |

### Data (WDATA, HRDATA, LRDATA)

#### Do not care

| Bit | Description |
| - | - |
| DATA31-DATA0 | 0 |

#### Motor current

| Bit | Description |
| - | - |
| DATA31-DATA0 | Motor current (A) as 32bit float |

#### Motor speed

| Bit | Description |
| - | - |
| DATA31-DATA0 | Motor speed (rad/s) as 32bit float |

#### Motor position

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

#### Motor feedback gain

| Bit | Description |
| - | - |
| DATA31-DATA0 | Feedback gain as 32bit float |

## Design sheets

+ [Controller](docs/Controller.md)
+ [Motor driver](docs/MotorDriver.md)
+ [Current sense amplifier](docs/CurrentSenseAmp.md)
