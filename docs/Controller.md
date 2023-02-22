# STM32F303K8T6 (STMicroelectronics)

## Pin assignment

(LQFP32 package)
| Pin number | Name | Alternative function | Description |
| - | - | - | - |
| 1 | VDD | - | +3.3V |
| 2 | PF0/OSC_IN | XTAL | Reserved |
| 3 | PF1/OSC_OUT | XTAL | Reserved |
| 4 | NRST | - | Pull-down (10nF) |
| 5 | VDDA/VREF+ | - | Pull-up |
| 6 | PA0 | TIM2_CH1 | Motor 1 rotary encoder A, pull-up (10kOhm) |
| 7 | PA1 | TIM15_CH1N | Motor 1 driver IN+ |
| 8 | PA2 | TIM15_CH1 | Motor 1 driver IN- |
|||||
| 9 | PA3 | - | NC |
| 10 | PA4 | SPI1_NSS | Raspberry Pi SPI0 CEx, pull-up (10kOhm) |
| 11 | PA5 | SPI1_SCK | Raspberry Pi SPI0 SCLK |
| 12 | PA6 | SPI1_MISO | Raspberry Pi SPI0 MISO |
| 13 | PA7 | SPI1_MOSI | Raspberry Pi SPI0 MOSI |
| 14 | PB0 | ADC1_IN11 | Motor 1 current sense amplifier OUT |
| 15 | PB1 | ADC1_IN12 | Motor 2 current sense amplifier OUT |
| 16 | VSS | - | GND |
|||||
| 17 | VDD | - | +3.3V |
| 18 | PA8 | - | NC |
| 19 | PA9 | - | NC |
| 20 | PA10 | GPIO_OUT | Motor 1 driver HEN, pull-down (10kOhm) |
| 21 | PA11 | GPIO_OUT | Motor 2 driver HEN, pull-down (10kOhm) |
| 22 | PA12 | TIM16_CH1 | Motor 2 driver IN+ |
| 23 | PA13 | SWDAT | ST-LINK |
| 24 | PA14 | SWCLK | ST-LINK |
|||||
| 25 | PA15 | I2C1_SCL | Reserved (Raspberry Pi I2C0 SCL) |
| 26 | PB3 | TIM2_CH2 | Motor 1 rotary encoder B, pull-up (10kOhm) |
| 27 | PB4 | TIM3_CH1 | Motor 1 rotary encoder A, pull-up (10kOhm) |
| 28 | PB5 | TIM3_CH2 | Motor 2 rotary encoder B, pull-up (10kOhm) |
| 29 | PB6 | TIM16_CH1N | Motor driver IN- |
| 30 | PB7 | I2C1_SDA | Reserved (Raspberry Pi I2C0 SDA) |
| 31 | BOOT0 | - | Pull-down (10kOhm) |
| 32 | VSS | - | GND |

## Design

### Requirements and conditions

### Parameters determined
