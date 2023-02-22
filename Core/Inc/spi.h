/*
 * spi.h
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32f3xx_hal.h"

#define SPI_RDATA_SIZE (2)

void spi_start(void);
void spi_update(void);

#endif /* INC_SPI_H_ */
