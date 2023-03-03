/*
 * board.h
 *
 *  Created on: Mar 3, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "util.h"

typedef enum {
	MOTOR1,
	MOTOR2,
	NUM_OF_MOTORS
} MOTOR_CHANNEL;

void board_start(void);
void board_update(void);
void board_current_feedback(void);
void board_encoder_overflow(const MOTOR_CHANNEL channel, const BOOL isdown);

#endif /* INC_BOARD_H_ */
