/*
 * pwm.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>

extern uint16_t *const pwm_reg_addr;

void pwm_set_supply_voltage(const uint8_t channel, const float voltage);
float pwm_get_supply_voltage(const uint8_t channel);
void pwm_set_voltage(const uint8_t channel, const float voltage);

#endif /* INC_PWM_H_ */
