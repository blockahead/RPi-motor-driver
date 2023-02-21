/*
 * pwm.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "pwm.h"

#define REG_MAX (800)
#define REG_ZERO (REG_MAX / 2)

extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;

static float supply_voltage[NUM_OF_PWMS] = { 1.0F, 1.0F };
static float target_voltage[NUM_OF_PWMS] = { 0.0F, 0.0F };

void pwm_set_supply_voltage(const uint8_t channel, const float voltage) {
	switch (channel) {
	case PWM1:
	case PWM2:
		/* Supply voltage must be greater than 1V */
		if (voltage < 1.0F) {
			supply_voltage[channel] = 1.0F;
		} else {
			supply_voltage[channel] = voltage;
		}

		break;

	default:
		break;
	}
}

float pwm_get_supply_voltage(const uint8_t channel) {
	switch (channel) {
	case PWM1:
	case PWM2:
		return supply_voltage[channel];

	default:
		return 0.0F;
	}
}

void pwm_set_voltage(const uint8_t channel, const float voltage) {
	switch (channel) {
	case PWM1:
	case PWM2:
		/* Saturate by supply voltage */
		if (voltage < -supply_voltage[channel]) {
			target_voltage[channel] = -supply_voltage[channel];
		} else if (supply_voltage[channel] < voltage) {
			target_voltage[channel] = supply_voltage[channel];
		} else {
			target_voltage[channel] = voltage;
		}

		break;

	default:
		break;
	}
}

void pwm_start(void) {
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, (uint32_t) REG_ZERO);
	__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, (uint32_t) REG_ZERO);
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim15, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim16, TIM_CHANNEL_1);
}

void pwm_command(void) {
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1,
			(uint32_t ) ((float) REG_ZERO * (1.0F + target_voltage[PWM1] / supply_voltage[PWM1])));
	__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1,
			(uint32_t ) ((float) REG_ZERO * (1.0F + target_voltage[PWM2] / supply_voltage[PWM2])));
}
