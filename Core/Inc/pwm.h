/*
 * pwm.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>

void pwm_set_supply_voltage(float voltage);
float pwm_get_supply_voltage(void);
uint32_t pwm_voltage_to_reg(float voltage);

#endif /* INC_PWM_H_ */
