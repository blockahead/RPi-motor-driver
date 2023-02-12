# RPi-motor-driver

## Overview

+ DC Motor driver board for Raspberry Pi
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

## Design sheets

+ [Controller](Docs/Controller.md)
+ [Motor driver](Docs/MotorDriver.md)
+ [Current sense amplifier](Docs/CurrentSenseAmp.md)
