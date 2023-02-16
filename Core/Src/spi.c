/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#include "spi.h"

#pragma pack(1)

typedef union {
	uint8_t u8[1];
	struct {
		uint8_t ADDRESS :4;
		uint8_t CHANNEL :1;
		uint8_t REWRITABLE :1;
		uint8_t START :1;
		uint8_t READY :1;
	};
} SPI_RQ;

typedef union {
	uint8_t u8[4];
	uint16_t u16[2];
	uint32_t u32;
	float f32;
} SPI_DATA;

#pragma pack() /* pack(1) */

#define FALSE (0)
#define TRUE (1)

#define STATE_STANDBY (0)
#define STATE_WAIT_DATA (1)

#define ADDR_MOTOR_CURRENT (0)
#define ADDR_MOTOR_SPEED (1)
#define ADDR_MOTOR_POSITION (2)
#define ADDR_RESERVED_1 (3)
#define ADDR_CONTROL_MODE (4)
#define ADDR_CONTROL_TARGET (5)
#define ADDR_MOTOR_SUPPLY_VOLTAGE (6)
#define ADDR_RESERVED_2 (7)
#define ADDR_CURRENT_FB_KP (8)
#define ADDR_CURRENT_FB_TI (9)
#define ADDR_SPEED_FB_KP (10)
#define ADDR_SPEED_FB_TI (11)
#define ADDR_POSITION_FB_KP (12)
#define ADDR_POSITION_FB_TI (13)
#define ADDR_POSITION_FB_TD (14)
#define ADDR_RESERVED_3 (15)

static uint8_t has_request = 0;
static SPI_RQ rq, buf;
static SPI_DATA dw, dr;

uint8_t *spi_tx_addr = buf.u8, *spi_rx_addr = rq.u8;
uint16_t spi_data_length = sizeof(SPI_RQ);

void spi_transition_to_rq_wait() {
	spi_tx_addr = buf.u8;
	spi_rx_addr = rq.u8;
	spi_data_length = sizeof(SPI_RQ);
}

void spi_transition_to_data_wait() {
	spi_tx_addr = dr.u8;
	spi_rx_addr = dw.u8;
	spi_data_length = sizeof(SPI_DATA);
}

void spi_respond(void) {
	if (has_request) {
		/* Data waiting state */

		/* TODO: set data into corresponding variables */

		spi_transition_to_rq_wait();

		has_request = FALSE;
	} else {
		/* Request waiting state */
		switch (rq.ADDRESS) {
		case ADDR_MOTOR_CURRENT:
			dr.u32 = 0x1000;
			break;

		case ADDR_MOTOR_SPEED:
			dr.u32 = 0x1001;
			break;

		case ADDR_MOTOR_POSITION:
			dr.u32 = 0x1002;
			break;

		case ADDR_RESERVED_1:
			dr.u32 = 0x1003;
			break;

		case ADDR_CONTROL_MODE:
			dr.u32 = 0x1004;
			break;

		case ADDR_CONTROL_TARGET:
			dr.u32 = 0x01000005;
			break;

		case ADDR_MOTOR_SUPPLY_VOLTAGE:
			dr.u32 = 0x1006;
			break;

		case ADDR_RESERVED_2:
			dr.u32 = 0x1007;
			break;

		case ADDR_CURRENT_FB_KP:
			dr.u32 = 0x1008;
			break;

		case ADDR_CURRENT_FB_TI:
			dr.u32 = 0x1009;
			break;

		case ADDR_SPEED_FB_KP:
			dr.u32 = 0x100A;
			break;

		case ADDR_SPEED_FB_TI:
			dr.u32 = 0x100B;
			break;

		case ADDR_POSITION_FB_KP:
			dr.u32 = 0x100C;
			break;

		case ADDR_POSITION_FB_TI:
			dr.u32 = 0x100D;
			break;

		case ADDR_POSITION_FB_TD:
			dr.u32 = 0x100E;
			break;

		case ADDR_RESERVED_3:
			dr.u32 = 0x100F;
			break;

		default:
			break;
		}

		spi_transition_to_data_wait();

		has_request = TRUE;
	}
}
