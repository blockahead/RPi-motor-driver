/*
 * csa.h
 *
 *  Created on: Feb 21, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_CSA_H_
#define INC_CSA_H_

#include "stm32f3xx_hal.h"

typedef enum {
	CSA1, CSA2, NUM_OF_CSAS
} CSA_CHANNEL;

float csa_get_voltage(const uint8_t channel);
float csa_get_current(const uint8_t channel);
void csa_start(void);

#endif /* INC_CSA_H_ */
