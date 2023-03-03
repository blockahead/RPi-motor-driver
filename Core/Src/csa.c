/*
 * csa.c
 *
 *  Created on: Feb 21, 2023
 *      Author: Kazuki Hamada
 */

#include "csa.h"

#define REG_MAX (0xFFF)
#define REFERENCE_VOLTAGE (3.3F)

extern ADC_HandleTypeDef hadc1;

static uint16_t reg[NUM_OF_CSAS];

static float csa_get_voltage(const CSA_CHANNEL channel) {
	switch (channel) {
	case CSA1:
	case CSA2:
		return REFERENCE_VOLTAGE * ((float) reg[channel] / (float) REG_MAX);

	default:
		return 0.0F;
	}
}

float csa_get_current(const CSA_CHANNEL channel) {
	switch (channel) {
	case CSA1:
	case CSA2:
		return csa_get_voltage(channel);

	default:
		return 0.0F;
	}
}

void csa_start(void) {
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) reg, NUM_OF_CSAS);
}
