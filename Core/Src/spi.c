/*
 * spi.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Kazuki Hamada
 */

#include "spi.h"

#include "state.h"

typedef enum {
	SPI_DTYPE_DO_NOT_CARE = 0U,
	SPI_DTYPE_MOTOR_CURRENT,
	SPI_DTYPE_MOTOR_SPEED,
	SPI_DTYPE_MOTOR_POSITION,
	SPI_DTYPE_CONTROL_MODE,
	SPI_DTYPE_CONTROL_TARGET,
	SPI_DTYPE_MOTOR_SUPPLY_VOLTAGE,
	SPI_DTYPE_RESERVED_1,
	SPI_DTYPE_CURRENT_FB_KP,
	SPI_DTYPE_CURRENT_FB_TI,
	SPI_DTYPE_SPEED_FB_KP,
	SPI_DTYPE_SPEED_FB_TI,
	SPI_DTYPE_POSITION_FB_KP,
	SPI_DTYPE_POSITION_FB_TI,
	SPI_DTYPE_POSITION_FB_TD,
	SPI_DTYPE_RESERVED_2
} SPI_DTYPE;

#pragma pack(1)

/* 8bit */
typedef struct {
	uint8_t DTYPE :4;
	uint8_t RESERVED :3;
	uint8_t CHANNEL :1;
} SPI_REQ;

/* 32bit */
typedef union {
	int8_t i8[4];
	uint8_t u8[4];
	int16_t i16[2];
	uint16_t u16[2];
	int32_t i32;
	uint32_t u32;
	float f32;
} SPI_DATA;

/* 80bit */
typedef struct {
	uint8_t READY[2];
	uint8_t START;
	SPI_REQ RREQ[SPI_RDATA_SIZE];
	SPI_REQ WREQ;
	SPI_DATA WDATA;
} SPI_DW;

/* 80bit */
typedef struct {
	uint16_t TIME;
	SPI_DATA RDATA[SPI_RDATA_SIZE];
} SPI_DR;

typedef union {
	uint8_t u8[10];
	SPI_DW DW;
	SPI_DR DR;
} SPI_PACKET;

#pragma pack() /* pack(1) */

extern SPI_HandleTypeDef hspi1;
extern STATE state[];

static FlagStatus has_received = RESET;
static SPI_REQ RREQ[SPI_RDATA_SIZE];
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

static void spi_update_DW(void) {
	SPI_DTYPE DTYPE = DW.WREQ.DTYPE;
	uint8_t CHANNEL = DW.WREQ.CHANNEL;
	SPI_DATA *WDATA = &DW.WDATA;

	if (CHANNEL < NUM_OF_MOTORS) {
		switch (DTYPE) {
		case SPI_DTYPE_CONTROL_MODE:
			state[CHANNEL].control_mode = WDATA->u32;
			break;

		case SPI_DTYPE_CONTROL_TARGET:
			state[CHANNEL].control_target = WDATA->f32;
			break;

		case SPI_DTYPE_MOTOR_SUPPLY_VOLTAGE:
			state[CHANNEL].motor_supply_voltage = WDATA->f32;
			break;

		case SPI_DTYPE_CURRENT_FB_KP:
			state[CHANNEL].current_fbparam_Kp = WDATA->f32;
			break;

		case SPI_DTYPE_CURRENT_FB_TI:
			state[CHANNEL].current_fbparam_Ti = WDATA->f32;
			break;

		case SPI_DTYPE_SPEED_FB_KP:
			state[CHANNEL].speed_fbparam_Kp = WDATA->f32;
			break;

		case SPI_DTYPE_SPEED_FB_TI:
			state[CHANNEL].speed_fbparam_Ti = WDATA->f32;
			break;

		case SPI_DTYPE_POSITION_FB_KP:
			state[CHANNEL].position_fbparam_Kp = WDATA->f32;
			break;

		case SPI_DTYPE_POSITION_FB_TI:
			state[CHANNEL].position_fbparam_Ti = WDATA->f32;
			break;

		case SPI_DTYPE_POSITION_FB_TD:
			state[CHANNEL].position_fbparam_Td = WDATA->f32;
			break;

		default:
			/* Invalid address */
			break;
		}
	} else {
		/* CHANNEL must be less than the number of motor state */
	}
}

static void spi_update_DR(void) {
	/* Scanning lower and upper side of RREQ */
	for (uint8_t i = 0; i < SPI_RDATA_SIZE; i++) {
		SPI_DTYPE DTYPE = RREQ[i].DTYPE;
		uint8_t CHANNEL = RREQ[i].CHANNEL;
		SPI_DATA *RDATA = &DR.RDATA[i];

		switch (DTYPE) {
		case SPI_DTYPE_MOTOR_CURRENT:
			RDATA->f32 = state[CHANNEL].motor_current;
			break;

		case SPI_DTYPE_MOTOR_SPEED:
			RDATA->f32 = state[CHANNEL].motor_speed;
			break;

		case SPI_DTYPE_MOTOR_POSITION:
			RDATA->f32 = state[CHANNEL].motor_position;
			break;

		case SPI_DTYPE_CONTROL_MODE:
			RDATA->u32 = (uint32_t) state[CHANNEL].control_mode;
			break;

		case SPI_DTYPE_CONTROL_TARGET:
			RDATA->f32 = state[CHANNEL].control_target;
			break;

		case SPI_DTYPE_MOTOR_SUPPLY_VOLTAGE:
			RDATA->f32 = state[CHANNEL].motor_supply_voltage;
			break;

		case SPI_DTYPE_CURRENT_FB_KP:
			RDATA->f32 = state[CHANNEL].current_fbparam_Kp;
			break;

		case SPI_DTYPE_CURRENT_FB_TI:
			RDATA->f32 = state[CHANNEL].current_fbparam_Ti;
			break;

		case SPI_DTYPE_SPEED_FB_KP:
			RDATA->f32 = state[CHANNEL].speed_fbparam_Kp;
			break;

		case SPI_DTYPE_SPEED_FB_TI:
			RDATA->f32 = state[CHANNEL].speed_fbparam_Ti;
			break;

		case SPI_DTYPE_POSITION_FB_KP:
			RDATA->f32 = state[CHANNEL].position_fbparam_Kp;
			break;

		case SPI_DTYPE_POSITION_FB_TI:
			RDATA->f32 = state[CHANNEL].position_fbparam_Ti;
			break;

		case SPI_DTYPE_POSITION_FB_TD:
			RDATA->f32 = state[CHANNEL].position_fbparam_Td;
			break;

		default:
			/* Invalid address */
			RDATA->u32 = 0;
			break;
		}
	}
}

void spi_start(void) {
	has_received = RESET;
	HAL_SPI_TransmitReceive_IT(&hspi1, ((SPI_PACKET*) &DR)->u8,
			((SPI_PACKET*) &DW)->u8, sizeof(SPI_PACKET));
}

void spi_update(void) {
	if (has_received == SET && spi_check_packet() == SUCCESS) {
		spi_update_RREQ();
		spi_update_DW();
		spi_update_DR();
		spi_start();
	} else {
		/* Do nothing */
	}

	/* Set current state into TX buffer */
	spi_update_DR();
}
