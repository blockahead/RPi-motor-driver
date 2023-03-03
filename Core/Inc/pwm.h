/*
 * pwm.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "stm32f3xx_hal.h"

typedef enum {
	PWM1, PWM2, NUM_OF_PWMS
} PWM_CHANNEL;

void pwm_set_supply_voltage(const PWM_CHANNEL channel, const float voltage);
float pwm_get_supply_voltage(const PWM_CHANNEL channel);
void pwm_set_voltage(const PWM_CHANNEL channel, const float voltage);
void pwm_start(void);

#endif /* INC_PWM_H_ */
