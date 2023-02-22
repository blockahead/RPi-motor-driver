/*
 * state.h
 *
 *  Created on: Feb 17, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_STATE_H_
#define INC_STATE_H_

#include "stm32f3xx_hal.h"

typedef struct {
	float motor_current;
	float motor_speed;
	float motor_position;
	uint8_t control_mode;
	float control_target;
	float motor_supply_voltage;
	float current_fbparam_Kp;
	float current_fbparam_Ti;
	float speed_fbparam_Kp;
	float speed_fbparam_Ti;
	float position_fbparam_Kp;
	float position_fbparam_Ti;
	float position_fbparam_Td;
} STATE;

typedef enum {
	MOTOR1, MOTOR2, NUM_OF_MOTORS
} MOTOR_CHANNEL;

#endif /* INC_STATE_H_ */
