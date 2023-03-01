/*
 * pwm.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "pwm.h"

#include "util.h"

#define REG_MAX (800)
#define REG_ZERO (REG_MAX / 2)

extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;

static float supply_voltage[NUM_OF_PWMS] = { 1.0F, 1.0F };

void pwm_set_supply_voltage(const uint8_t channel, const float voltage) {
	switch (channel) {
	case PWM1:
	case PWM2:
		supply_voltage[channel] = saturation(voltage, 1.0F, voltage);

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
	float target_voltage;

	switch (channel) {
	case PWM1:
		target_voltage = saturation(voltage, -supply_voltage[PWM1],
				supply_voltage[PWM1]);
		__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1,
				(uint32_t ) ((float) REG_ZERO * (1.0F + target_voltage / supply_voltage[PWM1])));

		break;

	case PWM2:
		target_voltage = saturation(voltage, -supply_voltage[PWM2],
				supply_voltage[PWM2]);
		__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1,
				(uint32_t ) ((float) REG_ZERO * (1.0F + target_voltage / supply_voltage[PWM2])));

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
