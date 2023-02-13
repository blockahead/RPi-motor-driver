/*
 * pwm.c
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#include "pwm.h"

#define REG_MAX (400)

static float supply_voltage = 1.0;

void pwm_set_supply_voltage(float voltage) {
	supply_voltage = voltage;
}

float pwm_get_supply_voltage(void) {
	return supply_voltage;
}

uint32_t pwm_voltage_to_reg(float voltage) {
	if (voltage < -supply_voltage) {
		voltage = -supply_voltage;
	} else if (supply_voltage < voltage) {
		voltage = supply_voltage;
	}

	return (uint32_t) (REG_MAX / 2) * (1.0 + voltage / supply_voltage);
}
