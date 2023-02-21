/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#include "spi.h"

#include "state.h"

typedef enum {
	SPI_STATUS_DISABLE,
	SPI_STATUS_RQ_WAIT,
	SPI_STATUS_RQ_RECEIVED,
	SPI_STATUS_DATA_WAIT,
	SPI_STATUS_DATA_RECEIVED
} SPI_STATUS;

typedef enum {
	SPI_ADDR_MOTOR_CURRENT,
	SPI_ADDR_MOTOR_SPEED,
	SPI_ADDR_MOTOR_POSITION,
	SPI_ADDR_RESERVED_1,
	SPI_ADDR_CONTROL_MODE,
	SPI_ADDR_CONTROL_TARGET,
	SPI_ADDR_MOTOR_SUPPLY_VOLTAGE,
	SPI_ADDR_RESERVED_2,
	SPI_ADDR_CURRENT_FB_KP,
	SPI_ADDR_CURRENT_FB_TI,
	SPI_ADDR_SPEED_FB_KP,
	SPI_ADDR_SPEED_FB_TI,
	SPI_ADDR_POSITION_FB_KP,
	SPI_ADDR_POSITION_FB_TI,
	SPI_ADDR_POSITION_FB_TD,
	SPI_ADDR_RESERVED_3
} SPI_ADDR;

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

extern SPI_HandleTypeDef hspi1;
extern STATE state[];

static SPI_RQ rq, buf;
static SPI_DATA dw, dr;
static SPI_STATUS status = SPI_STATUS_DISABLE;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	switch (status) {
	case SPI_STATUS_RQ_WAIT:
		/* RQ packet received */
		status = SPI_STATUS_RQ_RECEIVED;

		break;
	case SPI_STATUS_DATA_WAIT:
		/* RQ packet received */
		status = SPI_STATUS_DATA_RECEIVED;

		break;
	default:
		break;
	}
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
	spi_start();
}

static ErrorStatus spi_rq_res(void) {
	if (!rq.READY && rq.START) {
		uint8_t channel = rq.CHANNEL;

		switch (rq.ADDRESS) {
		case SPI_ADDR_MOTOR_CURRENT:
			dr.f32 = state[channel].motor_current;
			return SUCCESS;

		case SPI_ADDR_MOTOR_SPEED:
			dr.f32 = state[channel].motor_speed;
			return SUCCESS;

		case SPI_ADDR_MOTOR_POSITION:
			dr.f32 = state[channel].motor_position;
			return SUCCESS;

		case SPI_ADDR_CONTROL_MODE:
			dr.u32 = (uint32_t) state[channel].control_mode;
			return SUCCESS;

		case SPI_ADDR_CONTROL_TARGET:
			dr.f32 = state[channel].control_target;
			return SUCCESS;

		case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
			dr.f32 = state[channel].motor_supply_voltage;
			return SUCCESS;

		case SPI_ADDR_CURRENT_FB_KP:
			dr.f32 = state[channel].current_fbgain_Kp;
			return SUCCESS;

		case SPI_ADDR_CURRENT_FB_TI:
			dr.f32 = state[channel].current_fbgain_Ti;
			return SUCCESS;

		case SPI_ADDR_SPEED_FB_KP:
			dr.f32 = state[channel].speed_fbgain_Kp;
			return SUCCESS;

		case SPI_ADDR_SPEED_FB_TI:
			dr.f32 = state[channel].speed_fbgain_Ti;
			return SUCCESS;

		case SPI_ADDR_POSITION_FB_KP:
			dr.f32 = state[channel].position_fbgain_Kp;
			return SUCCESS;

		case SPI_ADDR_POSITION_FB_TI:
			dr.f32 = state[channel].position_fbgain_Ti;
			return SUCCESS;

		case SPI_ADDR_POSITION_FB_TD:
			dr.f32 = state[channel].position_fbgain_Td;
			return SUCCESS;

		default:
			/* Invalid address */
			dr.u32 = 0;
			return ERROR;
		}
	} else {
		/* invalid RQ format */
		dr.u32 = 0;
		return ERROR;
	}
}

static ErrorStatus spi_data_res(void) {
	if (rq.REWRITABLE) {
		uint8_t channel = rq.CHANNEL;

		switch (rq.ADDRESS) {
		case SPI_ADDR_CONTROL_MODE:
			state[channel].control_mode = (uint8_t) dw.u32;
			return SUCCESS;

		case SPI_ADDR_CONTROL_TARGET:
			state[channel].control_target = dw.f32;
			return SUCCESS;

		case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
			state[channel].motor_supply_voltage = dw.f32;
			return SUCCESS;

		case SPI_ADDR_CURRENT_FB_KP:
			state[channel].current_fbgain_Kp = dw.f32;
			return SUCCESS;

		case SPI_ADDR_CURRENT_FB_TI:
			state[channel].current_fbgain_Ti = dw.f32;
			return SUCCESS;

		case SPI_ADDR_SPEED_FB_KP:
			state[channel].speed_fbgain_Kp = dw.f32;
			return SUCCESS;

		case SPI_ADDR_SPEED_FB_TI:
			state[channel].speed_fbgain_Ti = dw.f32;
			return SUCCESS;

		case SPI_ADDR_POSITION_FB_KP:
			state[channel].position_fbgain_Kp = dw.f32;
			return SUCCESS;

		case SPI_ADDR_POSITION_FB_TI:
			state[channel].position_fbgain_Ti = dw.f32;
			return SUCCESS;

		case SPI_ADDR_POSITION_FB_TD:
			state[channel].position_fbgain_Td = dw.f32;
			return SUCCESS;

		default:
			/* Invalid address */
			return ERROR;
		}
	} else {
		/* Request is read only */
		return SUCCESS;
	}
}

void spi_start(void) {
	HAL_SPI_TransmitReceive_IT(&hspi1, buf.u8, rq.u8, sizeof(SPI_RQ));
	status = SPI_STATUS_RQ_WAIT;
}

void spi_update(void) {
	switch (status) {
	case SPI_STATUS_RQ_RECEIVED:
		if (spi_rq_res() == SUCCESS) {
			/* Valid request */
			status = SPI_STATUS_DATA_WAIT;
			HAL_SPI_TransmitReceive_IT(&hspi1, dr.u8, dw.u8, sizeof(SPI_DATA));
		} else {
			/* Invalid request */
			status = SPI_STATUS_RQ_WAIT;
			HAL_SPI_TransmitReceive_IT(&hspi1, buf.u8, rq.u8, sizeof(SPI_RQ));
		}

		break;

	case SPI_STATUS_DATA_RECEIVED:
		if (spi_data_res() == SUCCESS) {
			/* Valid data */
			status = SPI_STATUS_RQ_WAIT;
			HAL_SPI_TransmitReceive_IT(&hspi1, buf.u8, rq.u8, sizeof(SPI_RQ));
		} else {
			/* Invalid data */
			status = SPI_STATUS_RQ_WAIT;
			HAL_SPI_TransmitReceive_IT(&hspi1, buf.u8, rq.u8, sizeof(SPI_RQ));
		}

		break;

	default:
		break;
	}
}
