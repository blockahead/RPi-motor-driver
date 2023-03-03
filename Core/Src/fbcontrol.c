/*
 * fbcontrol.c
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#include "fbcontrol.h"

float fbcontrol_pi(const float r, const float y, FBCONTROL_PARAM *param) {
	float e = r - y;
	param->ei += e * param->Ts;

	return param->Kp * e + param->Ki * param->ei;
}

float fbcontrol_pid(const float r, const float y, FBCONTROL_PARAM *param) {
	float e = r - y;
	float dydt = (param->y_pre - y) / param->Ts;

	param->ei += e * param->Ts;
	param->y_pre = y;

	return param->Kp * e + param->Ki * param->ei + param->Kd * dydt;
}
