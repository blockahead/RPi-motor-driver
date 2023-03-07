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

typedef enum {
	BOARD_CONTROL_MODE_DISABLE = 0U,
	BOARD_CONTROL_MODE_VOLTAGE,
	BOARD_CONTROL_MODE_CURRENT,
	BOARD_CONTROL_MODE_SPEED,
	BOARD_CONTROL_MODE_POSITION,
} BOARD_CONTROL_MODE;

typedef struct {
	PWM_CHANNEL pwm;
	CSA_CHANNEL csa;
	ENCODER_CHANNEL encoder;
} BOARD_PERIPHERAL_CHANNEL;

typedef struct {
	float motor_current;
	float motor_speed;
	float motor_position;
	BOARD_CONTROL_MODE control_mode;
	float control_target;
	float motor_supply_voltage;
	uint32_t motor_encoder_resolution;
	FBCONTROL_PARAM fbparam[NUM_OF_CONTROL_TYPES];
	SPI_ADDR transmit_data_address;
	BOARD_PERIPHERAL_CHANNEL periph;
	float target_current;
} BOARD_STATE;

static BOARD_STATE state[NUM_OF_MOTORS];

static void board_state_init(void) {
	state[MOTOR1].motor_supply_voltage = 0.0F;
	state[MOTOR1].fbparam[CURRENT].fbgain.Kp = 0.0F;
	state[MOTOR1].fbparam[CURRENT].fbgain.Ki = 0.0F;
	state[MOTOR1].periph.pwm = PWM1;
	state[MOTOR1].periph.csa = CSA1;
	state[MOTOR1].periph.encoder = ENCODER1;
	state[MOTOR1].fbparam[CURRENT].fbstate.Ts = 100.0e-6F;

	state[MOTOR2].motor_supply_voltage = 0.0F;
	state[MOTOR2].fbparam[CURRENT].fbgain.Kp = 0.0F;
	state[MOTOR2].fbparam[CURRENT].fbgain.Ki = 0.0F;
	state[MOTOR2].periph.pwm = PWM2;
	state[MOTOR2].periph.csa = CSA2;
	state[MOTOR2].periph.encoder = ENCODER2;
	state[MOTOR2].fbparam[CURRENT].fbstate.Ts = 100.0e-6F;
}

static void board_convert_spi2state(BOARD_STATE *s, const SPI_ADDR addr, const SPI_DATA data) {
	switch (addr) {
	case SPI_ADDR_CONTROL_MODE:
		s->control_mode = (BOARD_CONTROL_MODE) data.u32;
		break;

	case SPI_ADDR_CONTROL_TARGET:
		s->control_target = data.f32;
		break;

	case SPI_ADDR_MOTOR_SUPPLY_VOLTAGE:
		s->motor_supply_voltage = data.f32;
		break;

	case SPI_ADDR_MOTOR_ENCODER_RESOLUTION:
		s->motor_encoder_resolution = data.u32;
		break;

	case SPI_ADDR_CURRENT_FB_KP:
		s->fbparam[CURRENT].fbgain.Kp = data.f32;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		s->fbparam[CURRENT].fbgain.Ki = data.f32;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		s->fbparam[SPEED].fbgain.Kp = data.f32;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		s->fbparam[SPEED].fbgain.Ki = data.f32;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		s->fbparam[POSITION].fbgain.Kp = data.f32;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		s->fbparam[POSITION].fbgain.Ki = data.f32;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		s->fbparam[POSITION].fbgain.Kd = data.f32;
		break;

	case SPI_ADDR_RDATA_ADDRESS:
		s->transmit_data_address = (SPI_ADDR) data.u32;
		break;

	default:
		/* Invalid address */
		break;
	}
}

static void board_convert_state2spi(SPI_DATA *data, const BOARD_STATE *s) {
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
		data->f32 = s->fbparam[CURRENT].fbgain.Kp;
		break;

	case SPI_ADDR_CURRENT_FB_TI:
		data->f32 = s->fbparam[CURRENT].fbgain.Ki;
		break;

	case SPI_ADDR_SPEED_FB_KP:
		data->f32 = s->fbparam[SPEED].fbgain.Kp;
		break;

	case SPI_ADDR_SPEED_FB_TI:
		data->f32 = s->fbparam[SPEED].fbgain.Ki;
		break;

	case SPI_ADDR_POSITION_FB_KP:
		data->f32 = s->fbparam[POSITION].fbgain.Kp;
		break;

	case SPI_ADDR_POSITION_FB_TI:
		data->f32 = s->fbparam[POSITION].fbgain.Ki;
		break;

	case SPI_ADDR_POSITION_FB_TD:
		data->f32 = s->fbparam[POSITION].fbgain.Kd;
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

static void board_update_spi(void) {
	/* Peripheral to state */
	for (uint8_t i = 0; i < NUM_OF_MOTORS; i++) {
		state[i].motor_current = csa_get_current(state[i].periph.csa);
		state[i].motor_position = encoder_get_angle_rad(state[i].periph.encoder);
	}

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
		/* These function calls must placed after the data is received to send   */
		/* the latest data corresponding received address if address has changed */
		board_convert_state2spi(&data1, &state[MOTOR1]);
		board_convert_state2spi(&data2, &state[MOTOR2]);
		spi_set_DR((uint16_t) 0, data1, data2);

		/* Restart receiving data */
		spi_start();

		/* State to peripheral */
		for (uint8_t i = 0; i < NUM_OF_MOTORS; i++) {
			encoder_set_pulse_per_rev(state[i].periph.encoder, state[i].motor_encoder_resolution);
			pwm_set_supply_voltage(state[i].periph.pwm, state[i].motor_supply_voltage);
		}
	} else {
		/* Do nothing */
	}
}

static void board_update_feedback() {
	float tmp;

	for (uint8_t i = 0; i < NUM_OF_MOTORS; i++) {
		switch (state[i].control_mode) {
		case BOARD_CONTROL_MODE_VOLTAGE:
			state[i].target_current = 0.0F;
			pwm_set_voltage(state[i].periph.pwm, state[i].control_target);
			break;

		case BOARD_CONTROL_MODE_CURRENT:
			state[i].target_current = state[i].control_target;
			break;

		case BOARD_CONTROL_MODE_SPEED:
			state[i].target_current = fbcontrol_pi(state[i].control_target, state[i].motor_speed, &state[i].fbparam[SPEED]);
			break;

		case BOARD_CONTROL_MODE_POSITION:
			tmp = fbcontrol_pid(state[i].control_target, state[i].motor_position, &state[i].fbparam[POSITION]);
			state[i].target_current = fbcontrol_pi(tmp, state[i].motor_speed, &state[i].fbparam[SPEED]);
			break;

		default:
			state[i].target_current = 0.0F;
			break;
		}
	}
}

void board_update(void) {
	board_update_spi();
	board_update_feedback();
}

void board_current_feedback(const MOTOR_CHANNEL channel) {
	float r, y, u;

	switch (state[channel].control_mode) {
	case BOARD_CONTROL_MODE_CURRENT:
	case BOARD_CONTROL_MODE_SPEED:
	case BOARD_CONTROL_MODE_POSITION:
		r = state[channel].target_current;
		y = csa_get_current(state[channel].periph.csa);
		u = fbcontrol_pi(r, y, &state[channel].fbparam[CURRENT]);
		pwm_set_voltage(state[channel].periph.pwm, u);
		break;

	default:
		break;
	}
}

void board_encoder_overflow_handler(const MOTOR_CHANNEL channel, const BOOL isdown) {
	if (isdown) {
		encoder_count_overflow(state[channel].periph.encoder, -1);
	} else {
		encoder_count_overflow(state[channel].periph.encoder, 1);
	}
}

void board_spi_receive_handler(const BOOL issuccess) {
	if (issuccess) {
		spi_receive();
	} else {
		spi_error();
	}
}
