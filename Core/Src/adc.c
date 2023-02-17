/*
 * adc.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "adc.h"

#define NUM_OF_CHANNELS (2)
#define REG_MAX (0xFFF)

static uint16_t reg[NUM_OF_CHANNELS];
static float reference_voltage[NUM_OF_CHANNELS];

uint16_t *const adc_reg_addr = reg;

uint8_t adc_get_num_of_channels(void) {
	return NUM_OF_CHANNELS;
}

void adc_set_reference_voltage(const uint8_t channel, const float voltage) {
	if (channel < NUM_OF_CHANNELS) {
		/* Reference voltage must be positive */
		if (voltage < 0) {
			reference_voltage[channel] = 0.0;
		} else {
			reference_voltage[channel] = voltage;
		}
	} else {
		/* Do nothing */
	}
}

float adc_get_reference_voltage(const uint8_t channel) {
	if (channel < NUM_OF_CHANNELS) {
		return reference_voltage[channel];
	} else {
		return 0.0;
	}
}

float adc_get_voltage(const uint8_t channel) {
	if (channel < NUM_OF_CHANNELS) {
		return reference_voltage[channel]
				* ((float) reg[channel] / (float) REG_MAX);
	} else {
		return 0.0;
	}
}
