/*
 * spi.h
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdint.h>

extern uint8_t *spi_tx_addr, *spi_rx_addr;
extern uint16_t spi_data_length;

void spi_respond(void);

#endif /* INC_SPI_H_ */
