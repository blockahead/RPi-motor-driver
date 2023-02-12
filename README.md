# RPi-motor-driver

## Overview

+ DC Motor driver board for Raspberry Pi
  + Locked anti phase (LAP) PWM
  + Contollable by voltage / current (torque)
  + only Raspberry Pi 4B tested
+ CPU and peripherals
  + F3 series CPU (STM32F303K8T6; STMicroelectronics)
  + Current sense amplifier (MAX4071; Maxim Integrated)
  + Full bridge FET driver (HIP4080A; Renesas)
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

## Pin assignment

### STM32F303K8T6 (LQFP32)

| Pin number | Name | Alternative function | Description |
| - | - | - | - |
| 1 | VDD | - | +3.3V |
| 2 | PF0/OSC_IN | XTAL | Reserved |
| 3 | PF1/OSC_OUT | XTAL | Reserved |
| 4 | NRST | - | Pulldown (10nF) |
| 5 | VDDA/VREF+ | - | +3.3V |
| 6 | PA0 | TIM2_CH1 | Motor 1 rotary encoder A |
| 7 | PA1 | TIM15_CH1N | Motor 1 PWM + |
| 8 | PA2 | TIM15_CH1 | Motor 1 PWM - |
|||||
| 9 | PA3 | - | NC |
| 10 | PA4 | SPI1_NSS | Raspberry Pi SPI |
| 11 | PA5 | SPI1_SCK | Raspberry Pi SPI |
| 12 | PA6 | SPI1_MISO | Raspberry Pi SPI |
| 13 | PA7 | SPI1_MOSI | Raspberry Pi SPI |
| 14 | PB0 | ADC1_IN11 | Motor 1 current sense amplifier |
| 15 | PB1 | ADC1_IN12 | Motor 2 current sense amplifier |
| 16 | VSS | - | GND |
|||||
| 17 | VDD | - | +3.3V |
| 18 | PA8 | - | NC |
| 19 | PA9 | - | NC |
| 20 | PA10 | - | NC |
| 21 | PA11 | - | NC |
| 22 | PA12 | TIM16_CH1 | Motor 2 PWM + |
| 23 | PA13 | SWDAT | ST-LINK |
| 24 | PA14 | SWCLK | ST-LINK |
|||||
| 25 | PA15 | I2C1_SCL | Reserved |
| 26 | PB3 | TIM2_CH2 | Motor 1 rotary encoder B |
| 27 | PB4 | TIM3_CH1 | Motor 1 rotary encoder A |
| 28 | PB5 | TIM3_CH2 | Motor 2 rotary encoder B |
| 29 | PB6 | TIM16_CH1N | Motor 2 PWM - |
| 30 | PB7 | I2C1_SDA | Reserved |
| 31 | BOOT0 | - | Pulldown (10kOhm) |
| 32 | VSS | - | GND |
|||||

### HIP4080A

| Pin number | Name | Description |
| - | - | - |
| 1 | BHB |
| 2 | HEN |
| 3 | DIS |
| 4 | VSS |
| 5 | OUT |
| 6 | IN+ |
| 7 | IN- |
| 8 | HDEL |
| 9 | LDEL |
| 10 | AHB |
||||
| 11 | AHO |
| 12 | AHS |
| 13 | ALO |
| 14 | ALS |
| 15 | VCC |
| 16 | VDD |
| 17 | BLS |
| 18 | BLO |
| 19 | BHS |
| 20 | BHO |
||||

### MAX4071

+ $V_{REF} = 1.5 \mathrm{V}$
+ $V = -3 \mathrm{A} \sim +3 \mathrm{A}$
+ $R_{SENSE} = 10 \mathrm{m \Omega}$
+ $V_{SENSE} = -30 \mathrm{mV} \sim +30 \mathrm{mV}$
+ $Gain = 50 \mathrm{x}$
+ $V_{OUT} = (V_{SENSE} \times Gain) + V_{REF} = 0 \mathrm{V} \sim 3 \mathrm{V}$

| Pin number | Name | Description |
| - | - | - |
| 1 | nSHDN | +3.3V |
| 2 | RS- | RSENSE - (10mOhm) |
| 3 | RS+ | RSENSE + (10mOhm) |
| 4 | GND | GND |
||||
| 5 | REFOUT | NC |
| 6 | OUT | Controller ADC |
| 7 | VSS | +3.3V |
| 8 | GSEL | GND (50V/V gain) |
||||
