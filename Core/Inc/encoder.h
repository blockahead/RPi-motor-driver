/*
 * encoder.h
 *
 *  Created on: Feb 20, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdint.h>

void encoder_ofuf_count_inc(const uint8_t channel);
void encoder_ofuf_count_dec(const uint8_t channel);

#endif /* INC_ENCODER_H_ */
