/*
 * encoder.h
 *
 *  Created on: Feb 20, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32f3xx_hal.h"

typedef enum {
	ENCODER1 = 0U,
	ENCODER2,
	NUM_OF_ENCODERS
} ENCODER_CHANNEL;

void encoder_count_overflow(const ENCODER_CHANNEL encoder, const int8_t dir);
void encoder_clear_count(const ENCODER_CHANNEL channel);
void encoder_set_pulse_per_rev(const ENCODER_CHANNEL channel, const uint16_t ppr);
float encoder_get_angle_rad(const ENCODER_CHANNEL channel);
void encoder_start(void);

#endif /* INC_ENCODER_H_ */
