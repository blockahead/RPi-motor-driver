/*
 * pwm.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "pwm.h"

#include <math.h>
#include <float.h>

#define NUM_OF_CHANNELS (2)
#define REG_MAX (400)
#define REG_ZERO (REG_MAX / 2)

static uint16_t reg[NUM_OF_CHANNELS];
static float supply_voltage[NUM_OF_CHANNELS];

uint16_t *const pwm_reg_addr = reg;

uint8_t pwm_get_num_of_channels(void) {
	return NUM_OF_CHANNELS;
}

void pwm_set_supply_voltage(const uint8_t channel, const float voltage) {
	if (channel < NUM_OF_CHANNELS) {
		/* Supply voltage must be positive */
		if (voltage < 0.0F) {
			supply_voltage[channel] = 0.0F;
		} else {
			supply_voltage[channel] = voltage;
		}
	} else {
		/* Do nothing */
	}
}

float pwm_get_supply_voltage(const uint8_t channel) {
	if (channel < NUM_OF_CHANNELS) {
		return supply_voltage[channel];
	} else {
		return 0.0F;
	}
}

void pwm_set_voltage(const uint8_t channel, const float voltage) {
	if (channel < NUM_OF_CHANNELS) {
		float target_voltage;

		/* Saturate by supply voltage */
		if (voltage < -supply_voltage[channel]) {
			target_voltage = -supply_voltage[channel];
		} else if (supply_voltage[channel] < voltage) {
			target_voltage = supply_voltage[channel];
		} else {
			target_voltage = voltage;
		}

		/* Avoiding zero division (not allowed supply voltage is zero) */
		if (FLT_EPSILON < fabsf(supply_voltage[channel])) {
			reg[channel] = (uint16_t) ((float) REG_ZERO
					* (1.0F + target_voltage / supply_voltage[channel]));
		} else {
			reg[channel] = REG_ZERO;
		}
	} else {
		/* Do nothing */
	}
}
