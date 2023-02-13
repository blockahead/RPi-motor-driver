/*
 * adc.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "adc.h"

#define ADC_REG_MAX (0xFFF)

static float reference_voltage = 0.0;

void adc_set_reference_voltage(float voltage) {
	reference_voltage = voltage;
}

float adc_get_reference_voltage(void) {
	return reference_voltage;
}

float adc_get_voltage(uint16_t reg_value) {
	return reference_voltage * ((float) reg_value / (float) ADC_REG_MAX);
}
