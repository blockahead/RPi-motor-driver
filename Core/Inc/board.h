/*
 * board.h
 *
 *  Created on: Mar 3, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

typedef enum {
	MOTOR1,
	MOTOR2,
	NUM_OF_MOTORS
} MOTOR_CHANNEL;

void board_start(void);
void board_update(void);
void board_current_feedback(void);

#endif /* INC_BOARD_H_ */
