/*
 * state.h
 *
 *  Created on: Feb 17, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_STATE_H_
#define INC_STATE_H_

#include <stdint.h>

typedef struct {
	float motor_current;
	float motor_speed;
	float motor_position;
	uint8_t control_mode;
	float control_target;
	float motor_supply_voltage;
	float current_fbgain_Kp;
	float current_fbgain_Ti;
	float speed_fbgain_Kp;
	float speed_fbgain_Ti;
	float position_fbgain_Kp;
	float position_fbgain_Ti;
	float position_fbgain_Td;
} STATE;

extern STATE state[];

uint8_t state_get_num_of_channels(void);
void state_update_motor_current(const float current);
void state_update_motor_speed(const float speed);
void state_update_motor_position(const float position);

#endif /* INC_STATE_H_ */
