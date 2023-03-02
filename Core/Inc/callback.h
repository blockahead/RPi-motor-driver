/*
 * callback.h
 *
 *  Created on: Mar 2, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_CALLBACK_H_
#define INC_CALLBACK_H_

#include "encoder.h"
#include "spi.h"
#include "current_feedback.h"

/* Timer overflow/underflow interruption */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance->CR1 & TIM_CR1_CEN_Msk) {
		if (htim->Instance == TIM17) {
			/* 100us timer interruption */
			current_feedback();
		} else if (htim->Instance == TIM2) {
			/* Encoder1 overflow/underflow */
			encoder_ofuf(ENCODER1, __HAL_TIM_IS_TIM_COUNTING_DOWN(htim));
		} else if (htim->Instance == TIM3) {
			/* Encoder1 overflow/underflow */
			encoder_ofuf(ENCODER2, __HAL_TIM_IS_TIM_COUNTING_DOWN(htim));
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
