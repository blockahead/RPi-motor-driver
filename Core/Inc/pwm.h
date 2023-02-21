/*
 * pwm.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>

typedef enum {
	PWM1, PWM2, NUM_OF_PWMS
} PWM_CHANNEL;

void pwm_set_supply_voltage(const uint8_t channel, const float voltage);
float pwm_get_supply_voltage(const uint8_t channel);
void pwm_set_voltage(const uint8_t channel, const float voltage);
void pwm_start(void);
void pwm_command(void);

#endif /* INC_PWM_H_ */
