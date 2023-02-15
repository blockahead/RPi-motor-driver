/*
 * spi.h
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdint.h>

uint8_t* spi_tx_addr(void);
uint8_t* spi_rx_addr(void);
uint16_t spi_data_length(void);
void spi_respond(void);

#endif /* INC_SPI_H_ */
