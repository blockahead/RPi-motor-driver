/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#include "spi.h"

#pragma pack(1)

/* 80bit */
typedef struct {
	uint8_t START;
	struct {
		SPI_ADDR ADDR1 :4;
		SPI_ADDR ADDR2 :4;
	};
	SPI_DATA WDATA2;
	SPI_DATA WDATA1;
} SPI_DW;

/* 80bit */
typedef struct {
	uint16_t TIME;
	SPI_DATA RDATA2;
	SPI_DATA RDATA1;
} SPI_DR;

typedef union {
	uint8_t u8[10];
	SPI_DW DW;
	SPI_DR DR;
} SPI_PACKET;

#pragma pack() /* pack(1) */

extern SPI_HandleTypeDef hspi1;

static BOOL hasreceived = FALSE;
static SPI_DW dw;
static SPI_DR dr;

void spi_receive(void) {
	hasreceived = TRUE;
}

void spi_error(void) {
	HAL_SPI_Abort(&hspi1);
	spi_start();
}

BOOL spi_packet_hasreceived(void) {
	return hasreceived;
}

BOOL spi_packet_isvalid(void) {
	if (dw.START == 0x78) {
		return TRUE;
	} else {
		/* Invalid start bit */
		return FALSE;
	}
}

void spi_get_DW(SPI_ADDR *addr1, SPI_ADDR *addr2, SPI_DATA *wdata1,
		SPI_DATA *wdata2) {
	*addr1 = dw.ADDR1;
	*addr2 = dw.ADDR2;
	*wdata1 = dw.WDATA1;
	*wdata2 = dw.WDATA2;
}

void spi_set_DR(const uint16_t time, const SPI_DATA rdata1,
		const SPI_DATA rdata2) {
	dr.TIME = time;
	dr.RDATA1 = rdata1;
	dr.RDATA2 = rdata2;
}

void spi_start(void) {
	hasreceived = FALSE;
	__HAL_SPI_ENABLE_IT(&hspi1, SPI_IT_ERR);
	HAL_SPI_TransmitReceive_IT(&hspi1, ((SPI_PACKET*) &dr)->u8,
			((SPI_PACKET*) &dw)->u8, sizeof(SPI_PACKET));
}

