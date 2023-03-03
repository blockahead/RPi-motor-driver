/*
 * callback.h
 *
 *  Created on: Mar 2, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_CALLBACK_H_
#define INC_CALLBACK_H_

#include "board.h"
#include "spi.h"

/* Timer overflow/underflow interruption */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance->CR1 & TIM_CR1_CEN_Msk) {
		if (htim->Instance == TIM17) {
			/* Check current feedback execution period START */
			HAL_GPIO_WritePin(PERIOD_CURRENT_FB_GPIO_Port, PERIOD_CURRENT_FB_Pin, GPIO_PIN_SET);

			/* 100us timer interruption */
			board_current_feedback();

			/* Check current feedback execution period END */
			HAL_GPIO_WritePin(PERIOD_CURRENT_FB_GPIO_Port, PERIOD_CURRENT_FB_Pin, GPIO_PIN_RESET);
		} else if (htim->Instance == TIM2) {
			/* Motor1 encoder overflow */
			board_encoder_overflow(MOTOR1, __HAL_TIM_IS_TIM_COUNTING_DOWN(htim));
		} else if (htim->Instance == TIM3) {
			/* Motor2 encoder overflow */
			board_encoder_overflow(MOTOR2, __HAL_TIM_IS_TIM_COUNTING_DOWN(htim));
		}
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	spi_receive();
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	spi_error();
}

#endif /* INC_CALLBACK_H_ */
