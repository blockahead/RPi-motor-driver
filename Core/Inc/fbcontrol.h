/*
 * fbcontrol.h
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_FBCONTROL_H_
#define INC_FBCONTROL_H_

typedef enum {
	CURRENT = 0U,
	SPEED,
	POSITION,
	NUM_OF_CONTROL_TYPES,
} FBCONTROL_TYPES;

typedef struct {
	float Kp;
	float Ki;
	float Kd;
} FBCONTROL_GAIN;

typedef struct {
	float Ts;
	float ei;
	float y_pre;
} FBCONTROL_STATE;

typedef struct {
	FBCONTROL_GAIN fbgain;
	FBCONTROL_STATE fbstate;
} FBCONTROL_PARAM;

float fbcontrol_pi(const float r, const float y, FBCONTROL_PARAM *param);
float fbcontrol_pid(const float r, const float y, FBCONTROL_PARAM *param);
void fbcontrol_state_clear(FBCONTROL_STATE *state);

#endif /* INC_FBCONTROL_H_ */
