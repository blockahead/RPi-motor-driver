/*
 * current_feedback.c
 *
 *  Created on: Mar 2, 2023
 *      Author: Kazuki Hamada
 */

#include "current_feedback.h"

#include "state.h"
#include "pwm.h"
#include "csa.h"

typedef struct {
	float Kp;
	float Ki;
	float ei;
} CURRENT_FB_PARAM;

/* Sampling period (s) */
static const float Ts = 100.0e-6F;
static CURRENT_FB_PARAM param[NUM_OF_MOTORS];

static float current_feedback_pi(const float r, const float y,
		CURRENT_FB_PARAM *param) {
	float e = r - y;
	param->ei += e * Ts;

	return param->Kp * e + param->Ki * param->ei;
}

void current_feedback_update_params(void){

}

void current_feedback(void) {
	pwm_set_voltage(PWM1,
			current_feedback_pi(0.0F, csa_get_current(CSA1), &param[MOTOR1]));
	pwm_set_voltage(PWM2,
			current_feedback_pi(0.0F, csa_get_current(CSA2), &param[MOTOR2]));
}
