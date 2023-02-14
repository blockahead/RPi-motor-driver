/*
 * spi.h
 *
 *  Created on: Feb 14, 2023
 *      Author: hamada
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdint.h>

#define SPI_DATA_BYTES (5)

void spi_data_start(void);
void spi_data_end(void);
uint8_t spi_does_wait_request(void);
uint8_t spi_does_wait_data(void);

#endif /* INC_SPI_H_ */
