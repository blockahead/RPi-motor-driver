/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#include "spi.h"

#include "state.h"

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
		/* Received data packet */

		/* TODO: set data into corresponding variables */

		spi_transition_to_rq_wait();

		has_request = FALSE;
	} else {
		/* Received request packet */
		uint8_t channel = rq.CHANNEL;

		switch (rq.ADDRESS) {
		case ADDR_MOTOR_CURRENT:
			dr.f32 = state[channel].motor_current;
			break;

		case ADDR_MOTOR_SPEED:
			dr.f32 = state[channel].motor_speed;
			break;

		case ADDR_MOTOR_POSITION:
			dr.f32 = state[channel].motor_position;
			break;

		case ADDR_RESERVED_1:
			dr.u32 = 0;
			break;

		case ADDR_CONTROL_MODE:
			dr.u32 = (uint32_t) state[channel].control_mode;
			break;

		case ADDR_CONTROL_TARGET:
			dr.f32 = state[channel].control_target;
			break;

		case ADDR_MOTOR_SUPPLY_VOLTAGE:
			dr.f32 = state[channel].motor_supply_voltage;
			break;

		case ADDR_RESERVED_2:
			dr.u32 = 0;
			break;

		case ADDR_CURRENT_FB_KP:
			dr.f32 = state[channel].current_fbgain_Kp;
			break;

		case ADDR_CURRENT_FB_TI:
			dr.f32 = state[channel].current_fbgain_Ti;
			break;

		case ADDR_SPEED_FB_KP:
			dr.f32 = state[channel].speed_fbgain_Kp;
			break;

		case ADDR_SPEED_FB_TI:
			dr.f32 = state[channel].speed_fbgain_Ti;
			break;

		case ADDR_POSITION_FB_KP:
			dr.f32 = state[channel].position_fbgain_Kp;
			break;

		case ADDR_POSITION_FB_TI:
			dr.f32 = state[channel].position_fbgain_Ti;
			break;

		case ADDR_POSITION_FB_TD:
			dr.f32 = state[channel].position_fbgain_Td;
			break;

		case ADDR_RESERVED_3:
			dr.u32 = 0;
			break;

		default:
			break;
		}

		spi_transition_to_data_wait();

		has_request = TRUE;
	}
}
