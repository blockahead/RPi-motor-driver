/*
 * fbcontrol.h
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_FBCONTROL_H_
#define INC_FBCONTROL_H_

typedef enum {
	FBCONTROL_MODE_DISABLE,
	FBCONTROL_MODE_VOLTAGE,
	FBCONTROL_MODE_CURRENT,
	FBCONTROL_MODE_SPEED,
	FBCONTROL_MODE_POSITION,
} FBCONTROL_MODE;

typedef struct {
	float Ts;
	float Kp;
	float Ki;
	float Kd;
	float ei;
	float y_pre;
} FBCONTROL_PARAM;

float fbcontrol_pi(const float r, const float y, FBCONTROL_PARAM *param);
float fbcontrol_pid(const float r, const float y, FBCONTROL_PARAM *param);

#endif /* INC_FBCONTROL_H_ */
