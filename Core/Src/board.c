/*
 * board.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Kazuki Hamada
 */

#include "board.h"

#include "timer.h"
#include "encoder.h"
#include "pwm.h"
#include "csa.h"
#include "spi.h"
#include "fbcontrol.h"

typedef struct {
	float motor_current;
	float motor_speed;
	float motor_position;
	FBCONTROL_MODE control_mode;
	float control_target;
	float motor_supply_voltage;
	uint32_t motor_encoder_resolution;
	float current_fbparam_Kp;
	float current_fbparam_Ti;
	float speed_fbparam_Kp;
	float speed_fbparam_Ti;
	float position_fbparam_Kp;
	float position_fbparam_Ti;
	float position_fbparam_Td;
	SPI_ADDR transmit_data_address;
	PWM_CHANNEL pwm;
	CSA_CHANNEL csa;
	ENCODER_CHANNEL encoder;
} STATE;

static STATE state[NUM_OF_MOTORS];
static FBCONTROL_PARAM fbparam[NUM_OF_MOTORS];

static void board_state_init(void) {
	state[MOTOR1].motor_supply_voltage = 0.0F;
	state[MOTOR1].current_fbparam_Kp = 0.0F;
	state[MOTOR1].current_fbparam_Ti = 0.0F;
	state[MOTOR1].pwm = PWM1;
	state[MOTOR1].csa = CSA1;
	state[MOTOR1].encoder = ENCODER1;

	state[MOTOR2].motor_supply_voltage = 0.0F;
	state[MOTOR2].current_fbparam_Kp = 0.0F;
	state[MOTOR2].current_fbparam_Ti = 0.0F;
	state[MOTOR2].pwm = PWM2;
	state[MOTOR2].csa = CSA2;
	state[MOTOR2].encoder = ENCODER2;
}

static void board_convert_spi2state(STATE *state, const SPI_ADDR addr, const SPI_DATA data) {
	switch (addr) {
	case SPI_ADDR_CONTROL_MODE:
		state->control_mode = (FBCONTROL_MODE) data.u32;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		state->control_target = data.f32;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		state->motor_supply_voltage = data.f32;
		break;

	case SPI_ADDR_MOTOR_ENCODER_RESOLUTION:
		state->motor_encoder_resolution = data.u32;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		state->current_fbparam_Kp = data.f32;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		state->current_fbparam_Ti = data.f32;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		state->speed_fbparam_Kp = data.f32;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		state->speed_fbparam_Ti = data.f32;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		state->position_fbparam_Kp = data.f32;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		state->position_fbparam_Ti = data.f32;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		state->position_fbparam_Td = data.f32;
		break;

	case SPI_ADDR_RDATA_ADDRESS:
		state->transmit_data_address = (SPI_ADDR) data.u32;
		break;

	default:
		/* Invalid address */
		break;
	}
}

static void board_convert_state2spi(SPI_DATA *data, const STATE *s) {
	switch (s->transmit_data_address) {
	case SPI_ADDR_MOTOR_CURRENT:
		data->f32 = s->motor_current;
		break;

	case SPI_ADDR_MOTOR_SPEED:
		data->f32 = s->motor_speed;
		break;

	case SPI_ADDR_MOTOR_POSITION:
		data->f32 = s->motor_position;
		break;

	case SPI_ADDR_CONTROL_MODE:
		data->u32 = (uint32_t) s->control_mode;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		data->f32 = s->control_target;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		data->f32 = s->motor_supply_voltage;
		break;

	case SPI_ADDR_MOTOR_ENCODER_RESOLUTION:
		data->u32 = s->motor_encoder_resolution;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		data->f32 = s->current_fbparam_Kp;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		data->f32 = s->current_fbparam_Ti;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		data->f32 = s->speed_fbparam_Kp;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		data->f32 = s->speed_fbparam_Ti;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		data->f32 = s->position_fbparam_Kp;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		data->f32 = s->position_fbparam_Ti;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		data->f32 = s->position_fbparam_Td;
		break;

	case SPI_ADDR_RDATA_ADDRESS:
		data->u32 = (uint32_t) s->transmit_data_address;
		break;

	default:
		/* Invalid address */
		data->u32 = 0;
		break;
	}
}

void board_start(void) {
	board_state_init();

	/* Start peripherals */
	timer_start();
	encoder_start();
	pwm_start();
	csa_start();
	spi_start();
}

void board_update(void) {
	/* Peripheral to state */
	state[MOTOR1].motor_current = csa_get_current(CSA1);
	state[MOTOR2].motor_current = csa_get_current(CSA2);
	state[MOTOR1].motor_position = encoder_get_angle_rad(ENCODER1);
	state[MOTOR2].motor_position = encoder_get_angle_rad(ENCODER2);

	/* Analyze and send SPI packet */
	if (spi_packet_hasreceived()) {
		SPI_ADDR addr1, addr2;
		SPI_DATA data1, data2;

		if (spi_packet_isvalid()) {
			/* Update state if received data is valid */
			spi_get_DW(&addr1, &addr2, &data1, &data2);
			board_convert_spi2state(&state[MOTOR1], addr1, data1);
			board_convert_spi2state(&state[MOTOR2], addr2, data2);
		} else {
			/* Do nothing */
		}

		/* Set current state into transmit buffer */
		board_convert_state2spi(&data1, &state[MOTOR1]);
		board_convert_state2spi(&data2, &state[MOTOR2]);
		spi_set_DR((uint16_t) 0, data1, data2);

		/* Restart receiving data */
		spi_start();
	} else {
		/* Do nothing */
	}

	/* State to peripheral */
	encoder_set_pulse_per_rev(ENCODER1, state[MOTOR1].motor_encoder_resolution);
	encoder_set_pulse_per_rev(ENCODER2, state[MOTOR2].motor_encoder_resolution);
	pwm_set_supply_voltage(PWM1, state[MOTOR1].motor_supply_voltage);
	pwm_set_supply_voltage(PWM2, state[MOTOR2].motor_supply_voltage);

	/* State to controller */
	fbparam[MOTOR1].Ts = 100e-6F;
	fbparam[MOTOR1].Kp = state[MOTOR1].current_fbparam_Kp;
	if (isnonzero(state[MOTOR1].current_fbparam_Ti)) {
		fbparam[MOTOR1].Ki = state[MOTOR1].current_fbparam_Kp / state[MOTOR1].current_fbparam_Ti;
	}
}

void board_current_feedback(const MOTOR_CHANNEL channel) {
	float r, y, u;

	switch (state[channel].control_mode) {
	case FBCONTROL_MODE_CURRENT:
	case FBCONTROL_MODE_SPEED:
	case FBCONTROL_MODE_POSITION:
		r = 0.0F;
		y = csa_get_current(state[channel].csa);
		u = fbcontrol_pi(r, y, &fbparam[channel]);
		pwm_set_voltage(state[channel].pwm, u);
		break;

	default:
		break;
	}
}

void board_encoder_overflow_handler(const MOTOR_CHANNEL channel, const BOOL isdown) {
	if (isdown) {
		encoder_count_overflow(state[channel].encoder, -1);
	} else {
		encoder_count_overflow(state[channel].encoder, 1);
	}
}

void board_spi_receive_handler(const BOOL issuccess) {
	if (issuccess) {
		spi_receive();
	} else {
		spi_error();
	}
}
