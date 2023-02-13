/*
 * adc.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "adc.h"

#define REG_MAX (0xFFF)

static float reference_voltage = 0.0;

void adc_set_reference_voltage(float voltage) {
	reference_voltage = voltage;
}

float adc_get_reference_voltage(void) {
	return reference_voltage;
}

float adc_reg_to_voltage(uint32_t value) {
	return reference_voltage * ((float) value / (float) REG_MAX);
}
