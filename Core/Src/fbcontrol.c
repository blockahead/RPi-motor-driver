/*
 * fbcontrol.c
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#include "fbcontrol.h"

typedef struct {
	float Ts;
	float Kp;
	float Ki;
	float Kd;
	float ei;
	float x_pre;
} FBCONTROL_PARAM;

typedef enum {
	FBTYPE_CURRENT, FBTYPE_SPEED, FBTYPE_POSITION, NUM_OF_FBTYPES,
} FBTYPE;

static FBCONTROL_PARAM fbc[NUM_OF_MOTORS][NUM_OF_FBTYPES];

static float fbcontrol_pid(const float r, const float x, FBCONTROL_PARAM *param) {
	float e = r - x;
	float xd = (param->x_pre - x) / param->Ts;

	param->ei += e * param->Ts;
	param->x_pre = x;

	return param->Kp * e + param->Ki * param->ei + param->Kd * xd;
}

void fbcontrol_start(void) {
	for (uint8_t i = 0; i < NUM_OF_MOTORS; i++) {
		for (uint8_t j = 0; j < NUM_OF_FBTYPES; j++) {
			fbc[i][j].Ts = 1.0F;
			fbc[i][j].Kp = 0.0F;
			fbc[i][j].Ki = 0.0F;
			fbc[i][j].Kd = 0.0F;
			fbc[i][j].ei = 0.0F;
			fbc[i][j].x_pre = 0.0F;
		}
	}
}

float fbcontrol_update(const STATE state[]) {
	float control_input = 0.0F;

	fbcontrol_current();

	return 0.0F;
}
