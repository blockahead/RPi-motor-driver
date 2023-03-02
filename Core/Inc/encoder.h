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
	ENCODER1, ENCODER2, NUM_OF_ENCODERS
} ENCODER_CHANNEL;

void encoder_ofuf(const ENCODER_CHANNEL encoder, const FlagStatus is_down);
void encoder_clear_count(const uint8_t channel);
void encoder_set_pulse_per_rev(const uint8_t channel, const uint16_t ppr);
int32_t encoder_get_count(const uint8_t channel);
float encoder_get_angle_rad(const uint8_t channel);
void encoder_start(void);

#endif /* INC_ENCODER_H_ */
