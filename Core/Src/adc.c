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
static float reference_voltage = 0.0;

uint16_t *const adc_reg_addr = reg;

void adc_set_reference_voltage(const float voltage) {
	reference_voltage = voltage;
}

float adc_get_reference_voltage(void) {
	return reference_voltage;
}

uint8_t adc_get_num_of_channels(void) {
	return NUM_OF_CHANNELS;
}

float adc_get_voltage(const uint8_t channel) {
	float voltage = 0.0;

	if (channel < NUM_OF_CHANNELS) {
		voltage = reference_voltage * ((float) reg[channel] / (float) REG_MAX);
	}

	return voltage;
}
