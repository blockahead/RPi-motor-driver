/*
 * spi.h
 *
 *  Created on: Feb 14, 2023
 *      Author: hamada
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdint.h>

typedef union {
	uint8_t u8[1];
} SPI_Req;

typedef union {
	uint8_t u8[4];
	uint16_t u16[2];
	uint32_t u32;
	float f32;
} SPI_Data;

#endif /* INC_SPI_H_ */
