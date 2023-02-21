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

void encoder_clear_count(const uint8_t channel);
int32_t encoder_get_count(const uint8_t channel);
void encoder_start(void);

#endif /* INC_ENCODER_H_ */
