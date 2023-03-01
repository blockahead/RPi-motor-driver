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
static SPI_DW dw;
static SPI_DR dr;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	has_received = SET;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	HAL_SPI_Abort(&hspi1);
	spi_start();
}

static ErrorStatus spi_check_packet(void) {
	if (dw.START == 0x78) {
		return SUCCESS;
	} else {
		/* Invalid start bit */
		return ERROR;
	}
}

static ErrorStatus spi_check_rdata_address(const SPI_ADDR addr) {
	switch (addr) {
	case SPI_ADDR_MOTOR_CURRENT:
	case SPI_ADDR_MOTOR_SPEED:
	case SPI_ADDR_MOTOR_POSITION:
	case SPI_ADDR_CONTROL_MODE:
	case SPI_ADDR_CONTROL_TARGET:
	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
	case SPI_ADDR_CURRENT_FB_KP:
	case SPI_ADDR_CURRENT_FB_TI:
	case SPI_ADDR_SPEED_FB_KP:
	case SPI_ADDR_SPEED_FB_TI:
	case SPI_ADDR_POSITION_FB_KP:
	case SPI_ADDR_POSITION_FB_TI:
	case SPI_ADDR_POSITION_FB_TD:
	case SPI_ADDR_RDATA_ADDRESS:
		return SUCCESS;

	default:
		return ERROR;
	}
}

static void spi_update_state_sub(STATE state[], const SPI_DATA *wdata,
		const SPI_ADDR addr) {
	switch (addr) {
	case SPI_ADDR_CONTROL_MODE:
		state->control_mode = wdata->u32;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		state->control_target = wdata->f32;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		state->motor_supply_voltage = wdata->f32;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		state->current_fbparam_Kp = wdata->f32;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		state->current_fbparam_Ti = wdata->f32;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		state->speed_fbparam_Kp = wdata->f32;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		state->speed_fbparam_Ti = wdata->f32;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		state->position_fbparam_Kp = wdata->f32;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		state->position_fbparam_Ti = wdata->f32;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		state->position_fbparam_Td = wdata->f32;
		break;

	case SPI_ADDR_RDATA_ADDRESS:
		if (spi_check_rdata_address((SPI_ADDR) wdata->u32) == SUCCESS) {
			/* Update transmit data address */
			state->transmit_data_address = (uint8_t) wdata->u32;
		} else {
			/* Keep current address if request address is invalid */
		}

		break;

	default:
		/* Invalid address */
		break;
	}
}

static void spi_update_DR_TIME(void) {

}

static void spi_update_DR_RDATA(SPI_DATA *rdata, const STATE *state) {
	switch (state->transmit_data_address) {
	case SPI_ADDR_MOTOR_CURRENT:
		rdata->f32 = state->motor_current;
		break;

	case SPI_ADDR_MOTOR_SPEED:
		rdata->f32 = state->motor_speed;
		break;

	case SPI_ADDR_MOTOR_POSITION:
		rdata->f32 = state->motor_position;
		break;

	case SPI_ADDR_CONTROL_MODE:
		rdata->u32 = (uint32_t) state->control_mode;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		rdata->f32 = state->control_target;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		rdata->f32 = state->motor_supply_voltage;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		rdata->f32 = state->current_fbparam_Kp;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		rdata->f32 = state->current_fbparam_Ti;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		rdata->f32 = state->speed_fbparam_Kp;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		rdata->f32 = state->speed_fbparam_Ti;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		rdata->f32 = state->position_fbparam_Kp;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		rdata->f32 = state->position_fbparam_Ti;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		rdata->f32 = state->position_fbparam_Td;
		break;

	case SPI_ADDR_RDATA_ADDRESS:
		rdata->u32 = state->transmit_data_address;
		break;

	default:
		/* Invalid address */
		rdata->u32 = 0;
		break;
	}
}

static void spi_update_state(STATE state[]) {
	spi_update_state_sub(&state[MOTOR1], &dw.WDATA1, dw.ADDR1);
	spi_update_state_sub(&state[MOTOR2], &dw.WDATA2, dw.ADDR2);
}

static void spi_update_DR(const STATE state[]) {
	spi_update_DR_TIME();
	spi_update_DR_RDATA(&dr.RDATA1, &state[MOTOR1]);
	spi_update_DR_RDATA(&dr.RDATA2, &state[MOTOR2]);
}

void spi_start(void) {
	has_received = RESET;
	__HAL_SPI_ENABLE_IT(&hspi1, SPI_IT_ERR);
	HAL_SPI_TransmitReceive_IT(&hspi1, ((SPI_PACKET*) &dr)->u8,
			((SPI_PACKET*) &dw)->u8, sizeof(SPI_PACKET));
}

void spi_update(STATE state[]) {
	if (has_received == SET) {
		if (spi_check_packet() == SUCCESS) {
			/* Update state if received data is valid */
			spi_update_state(state);
		} else {
			/* Do nothing */
		}

		/* Set current state into transmit buffer */
		spi_update_DR(state);

		/* Restart receiving data */
		spi_start();
	} else {
		/* Do nothing */
	}
}
