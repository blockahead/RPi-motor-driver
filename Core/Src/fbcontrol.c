/*
 * fbcontrol.c
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#include "fbcontrol.h"

float fbcontrol_pi(const float r, const float y, FBCONTROL_PARAM *param) {
	float e = r - y;
	param->fbstate.ei += e * param->fbstate.Ts;

	return param->fbgain.Kp * e + param->fbgain.Ki * param->fbstate.ei;
}

float fbcontrol_pid(const float r, const float y, FBCONTROL_PARAM *param) {
	float e = r - y;
	float dydt = (param->fbstate.y_pre - y) / param->fbstate.Ts;

	param->fbstate.ei += e * param->fbstate.Ts;
	param->fbstate.y_pre = y;

	return param->fbgain.Kp * e + param->fbgain.Ki * param->fbstate.ei + param->fbgain.Kd * dydt;
}

void fbcontrol_state_clear(FBCONTROL_STATE *state) {
	state->ei = 0.0F;
	state->y_pre = 0.0F;
}
