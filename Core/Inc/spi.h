/*
 * spi.h
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32f3xx_hal.h"
#include "util.h"

typedef enum {
	SPI_ADDR_DO_NOT_CARE = 0U,
	SPI_ADDR_MOTOR_CURRENT,
	SPI_ADDR_MOTOR_SPEED,
	SPI_ADDR_MOTOR_POSITION,
	SPI_ADDR_CONTROL_MODE,
	SPI_ADDR_CONTROL_TARGET,
	SPI_ADDR_MOTOR_SUPPLY_VOLTAGE,
	SPI_ADDR_MOTOR_ENCODER_RESOLUTION,
	SPI_ADDR_CURRENT_FB_KP,
	SPI_ADDR_CURRENT_FB_TI,
	SPI_ADDR_SPEED_FB_KP,
	SPI_ADDR_SPEED_FB_TI,
	SPI_ADDR_POSITION_FB_KP,
	SPI_ADDR_POSITION_FB_TI,
	SPI_ADDR_POSITION_FB_TD,
	SPI_ADDR_RDATA_ADDRESS,
} SPI_ADDR;

#pragma pack(1)

/* 32bit */
typedef union {
	int32_t i32;
	uint32_t u32;
	float f32;
} SPI_DATA;

#pragma pack() /* pack(1) */

void spi_receive(void);
void spi_error(void);
BOOL spi_packet_hasreceived(void);
BOOL spi_packet_isvalid(void);
void spi_get_DW(SPI_ADDR *addr1, SPI_ADDR *addr2, SPI_DATA *wdata1,
		SPI_DATA *wdata2);
void spi_set_DR(const uint16_t time, const SPI_DATA rdata1,
		const SPI_DATA rdata2);
void spi_start(void);

#endif /* INC_SPI_H_ */
