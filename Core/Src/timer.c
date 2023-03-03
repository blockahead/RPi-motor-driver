/*
 * timer.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Kazuki Hamada
 */

#include "timer.h"

extern TIM_HandleTypeDef htim17;

void timer_start(void) {
	HAL_TIM_Base_Start_IT(&htim17);
}
