/*
 * callback.h
 *
 *  Created on: Mar 2, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_CALLBACK_H_
#define INC_CALLBACK_H_

#include "board.h"

/* Timer overflow/underflow interruption */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance->CR1 & TIM_CR1_CEN_Msk) {
		if (htim->Instance == TIM17) {
			/* Check current feedback execution period START */
			HAL_GPIO_WritePin(PERIOD_CURRENT_FB_GPIO_Port, PERIOD_CURRENT_FB_Pin, GPIO_PIN_SET);

			/* 100us timer interruption */
			board_current_feedback(MOTOR1);
			board_current_feedback(MOTOR2);

			/* Check current feedback execution period END */
			HAL_GPIO_WritePin(PERIOD_CURRENT_FB_GPIO_Port, PERIOD_CURRENT_FB_Pin, GPIO_PIN_RESET);
		} else if (htim->Instance == TIM2) {
			/* Motor1 encoder overflow */
			board_encoder_overflow_handler(MOTOR1, __HAL_TIM_IS_TIM_COUNTING_DOWN(htim));
		} else if (htim->Instance == TIM3) {
			/* Motor2 encoder overflow */
			board_encoder_overflow_handler(MOTOR2, __HAL_TIM_IS_TIM_COUNTING_DOWN(htim));
		}
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	BOOL issuccess = TRUE;
	board_spi_receive_handler(issuccess);
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	BOOL issuccess = FALSE;
	board_spi_receive_handler(issuccess);
}

#endif /* INC_CALLBACK_H_ */
