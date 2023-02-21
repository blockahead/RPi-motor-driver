/*
 * encoder.c
 *
 *  Created on: Feb 20, 2023
 *      Author: Kazuki Hamada
 */

#include "encoder.h"

#include "stm32f3xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static uint16_t ofuf_count[NUM_OF_ENCODERS] = { 0 };

/* Timer overflow/underflow interruption */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* Encoder 1 */
	if (htim->Instance == TIM2) {
		/* To avoid first interruption */
		if (htim->Instance->CR1 & TIM_CR1_CEN_Msk) {
			/* Check direction */
			if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)) {
				/* Counter underflow */
				ofuf_count[ENC1]--;
			} else {
				/* Counter overflow */
				ofuf_count[ENC1]++;
			}
		}
	}

	/* Encoder 2 */
	if (htim->Instance == TIM3) {
		/* To avoid first interruption */
		if (htim->Instance->CR1 & TIM_CR1_CEN_Msk) {
			/* Check direction */
			if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)) {
				/* Counter underflow */
				ofuf_count[ENC2]--;
			} else {
				/* Counter overflow */
				ofuf_count[ENC2]++;
			}
		}
	}
}

void encoder_clear_count(const uint8_t channel) {
	switch (channel) {
	case ENC1:
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		break;

	case ENC2:
		__HAL_TIM_SET_COUNTER(&htim3, 0);
		break;

	default:
		return;
	}

	ofuf_count[channel] = 0;
}

int32_t encoder_get_count(const uint8_t channel) {
	uint16_t lower;

	switch (channel) {
	case ENC1:
		lower = __HAL_TIM_GET_COUNTER(&htim2);
		break;

	case ENC2:
		lower = __HAL_TIM_GET_COUNTER(&htim3);
		break;

	default:
		return 0;
	}

	return (ofuf_count[channel] << 16) | lower;
}
