/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: hamada
 */

#include "spi.h"

typedef union {
	uint8_t u8[4];
	uint16_t u16[2];
	uint32_t u32;
	float f32;
} SPI_Data;

#define FALSE (0)
#define TRUE (1)
#define SPI_WAIT_REQUEST (0)
#define SPI_WAIT_DATA (1)

static uint8_t spi_state;

void spi_data_start(void) {
	spi_state = SPI_WAIT_DATA;
}

void spi_data_end(void) {
	spi_state = SPI_WAIT_REQUEST;
}

uint8_t spi_does_wait_request(void) {
	if (SPI_WAIT_REQUEST == spi_state) {
		return TRUE;
	} else {
		return FALSE;
	}
}

uint8_t spi_does_wait_data(void) {
	if (SPI_WAIT_DATA == spi_state) {
		return TRUE;
	} else {
		return FALSE;
	}
}
