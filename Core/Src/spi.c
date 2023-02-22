/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#include "spi.h"

typedef enum {
	SPI_ADDR_DO_NOT_CARE = 0U,
	SPI_ADDR_MOTOR_CURRENT,
	SPI_ADDR_MOTOR_SPEED,
	SPI_ADDR_MOTOR_POSITION,
	SPI_ADDR_CONTROL_MODE,
	SPI_ADDR_CONTROL_TARGET,
	SPI_ADDR_MOTOR_SUPPLY_VOLTAGE,
	SPI_ADDR_RESERVED_1,
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

/* 80bit */
typedef struct {
	uint8_t START;
	struct {
		uint8_t ADDR1 :4;
		uint8_t ADDR2 :4;
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

static FlagStatus has_received = RESET;
static SPI_DW DW;
static SPI_DR DR;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	has_received = SET;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	spi_start();
}

static ErrorStatus spi_check_packet(void) {
	if (DW.START == 0x78) {
		return SUCCESS;
	} else {
		/* Invalid start bit */
		return ERROR;
	}
}

/* Buffering RREQ */
static void spi_update_RREQ(void) {
	for (uint8_t i = 0; i < SPI_RDATA_SIZE; i++) {
		uint8_t CHANNEL = DW.RREQ[i].CHANNEL;

		if (CHANNEL < NUM_OF_MOTORS) {
			RREQ[i] = DW.RREQ[i];
		} else {
			/* CHANNEL must be less than the number of motor state */
		}
	}
}

static void spi_update_state_sub(STATE state[], const SPI_DATA *WDATA,
		const SPI_ADDR ADDR) {
	switch (ADDR) {
	case SPI_ADDR_CONTROL_MODE:
		state->control_mode = WDATA->u32;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		state->control_target = WDATA->f32;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		state->motor_supply_voltage = WDATA->f32;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		state->current_fbparam_Kp = WDATA->f32;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		state->current_fbparam_Ti = WDATA->f32;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		state->speed_fbparam_Kp = WDATA->f32;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		state->speed_fbparam_Ti = WDATA->f32;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		state->position_fbparam_Kp = WDATA->f32;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		state->position_fbparam_Ti = WDATA->f32;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		state->position_fbparam_Td = WDATA->f32;
		break;

	case SPI_ADDR_RDATA_ADDRESS:
		state->transmit_data_address = (uint8_t) WDATA->u32;

	default:
		/* Invalid address */
		break;
	}
}

static void spi_update_DR_TIME(void) {

}

static void spi_update_DR_RDATA(SPI_DATA *RDATA, const STATE *state) {
	switch (state->transmit_data_address) {
	case SPI_ADDR_MOTOR_CURRENT:
		RDATA->f32 = state->motor_current;
		break;

	case SPI_ADDR_MOTOR_SPEED:
		RDATA->f32 = state->motor_speed;
		break;

	case SPI_ADDR_MOTOR_POSITION:
		RDATA->f32 = state->motor_position;
		break;

	case SPI_ADDR_CONTROL_MODE:
		RDATA->u32 = (uint32_t) state->control_mode;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		RDATA->f32 = state->control_target;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		RDATA->f32 = state->motor_supply_voltage;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		RDATA->f32 = state->current_fbparam_Kp;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		RDATA->f32 = state->current_fbparam_Ti;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		RDATA->f32 = state->speed_fbparam_Kp;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		RDATA->f32 = state->speed_fbparam_Ti;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		RDATA->f32 = state->position_fbparam_Kp;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		RDATA->f32 = state->position_fbparam_Ti;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		RDATA->f32 = state->position_fbparam_Td;
		break;

	default:
		/* Invalid address */
		RDATA->u32 = 0;
		break;
	}
}

static void spi_update_state(STATE state[]) {
	spi_update_state_sub(&state[MOTOR1], &DW.WDATA1, DW.ADDR1);
	spi_update_state_sub(&state[MOTOR2], &DW.WDATA2, DW.ADDR2);
}

static void spi_update_DR(const STATE state[]) {
	spi_update_DR_TIME();
	spi_update_DR_RDATA(&DR.RDATA1, &state[MOTOR1]);
	spi_update_DR_RDATA(&DR.RDATA2, &state[MOTOR2]);
}

void spi_start(void) {
	has_received = RESET;
	HAL_SPI_TransmitReceive_IT(&hspi1, ((SPI_PACKET*) &DR)->u8,
			((SPI_PACKET*) &DW)->u8, sizeof(SPI_PACKET));
}

void spi_update(STATE state[]) {
	if (has_received == SET && spi_check_packet() == SUCCESS) {
		spi_update_RREQ();
		spi_update_state(state);
		spi_update_DR(state);
		spi_start();
	} else {
		/* Do nothing */
	}

	/* Set current state into transmit buffer */
	spi_update_DR(state);
}
