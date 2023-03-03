/*
 * encoder.c
 *
 *  Created on: Feb 20, 2023
 *      Author: Kazuki Hamada
 */

#include "encoder.h"

#define PI (3.14159265358979323846F)
#define TWOPI (PI * 2.0F)

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static uint16_t ofuf_count[NUM_OF_ENCODERS] = { 0, 0 };
static uint16_t pulse_per_rev[NUM_OF_ENCODERS] = { 0, 0 };

/* Timer overflow/underflow interruption */
void encoder_ofuf(const ENCODER_CHANNEL channel, const BOOL is_down) {
	if (is_down) {
		/* Counter underflow */
		ofuf_count[channel]--;
	} else {
		/* Counter overflow */
		ofuf_count[channel]++;
	}
}

void encoder_clear_count(const ENCODER_CHANNEL channel) {
	switch (channel) {
	case ENCODER1:
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		ofuf_count[ENCODER1] = 0;
		break;

	case ENCODER2:
		__HAL_TIM_SET_COUNTER(&htim3, 0);
		ofuf_count[ENCODER2] = 0;
		break;

	default:
		break;
	}
}

void encoder_set_pulse_per_rev(const ENCODER_CHANNEL channel, const uint16_t ppr) {
	switch (channel) {
	case ENCODER1:
	case ENCODER2:
		pulse_per_rev[channel] = ppr;
		break;

	default:
		break;
	}
}

static int32_t encoder_get_count(const ENCODER_CHANNEL channel) {
	switch (channel) {
	case ENCODER1:
		return (ofuf_count[ENCODER1] << 16) | __HAL_TIM_GET_COUNTER(&htim2);

	case ENCODER2:
		return (ofuf_count[ENCODER2] << 16) | __HAL_TIM_GET_COUNTER(&htim3);

	default:
		return 0;
	}
}

float encoder_get_angle_rad(const ENCODER_CHANNEL channel) {
	switch (channel) {
	case ENCODER1:
	case ENCODER2:
		return TWOPI * (float) encoder_get_count(channel) / (float) pulse_per_rev[channel];

	default:
		return 0.0F;
	}
}

void encoder_start(void) {
	__HAL_TIM_URS_ENABLE(&htim2);
	__HAL_TIM_URS_ENABLE(&htim3);
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}
