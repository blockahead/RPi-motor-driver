/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENC1_A_Pin GPIO_PIN_0
#define ENC1_A_GPIO_Port GPIOA
#define PWM1_N_Pin GPIO_PIN_1
#define PWM1_N_GPIO_Port GPIOA
#define PWM1_P_Pin GPIO_PIN_2
#define PWM1_P_GPIO_Port GPIOA
#define TEST_Pin GPIO_PIN_3
#define TEST_GPIO_Port GPIOA
#define SPI_NSS_Pin GPIO_PIN_4
#define SPI_NSS_GPIO_Port GPIOA
#define SPI_SCK_Pin GPIO_PIN_5
#define SPI_SCK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#define ADC1_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOB
#define ADC2_Pin GPIO_PIN_1
#define ADC2_GPIO_Port GPIOB
#define PERIOD_MAIN_Pin GPIO_PIN_8
#define PERIOD_MAIN_GPIO_Port GPIOA
#define PERIOD_CURRENT_FB_Pin GPIO_PIN_9
#define PERIOD_CURRENT_FB_GPIO_Port GPIOA
#define PWM1_EN_Pin GPIO_PIN_10
#define PWM1_EN_GPIO_Port GPIOA
#define PWM2_EN_Pin GPIO_PIN_11
#define PWM2_EN_GPIO_Port GPIOA
#define PWM2_P_Pin GPIO_PIN_12
#define PWM2_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define I2C_SCL_Pin GPIO_PIN_15
#define I2C_SCL_GPIO_Port GPIOA
#define ENC1_B_Pin GPIO_PIN_3
#define ENC1_B_GPIO_Port GPIOB
#define ENC2_A_Pin GPIO_PIN_4
#define ENC2_A_GPIO_Port GPIOB
#define ENC2_B_Pin GPIO_PIN_5
#define ENC2_B_GPIO_Port GPIOB
#define PWM2_N_Pin GPIO_PIN_6
#define PWM2_N_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
