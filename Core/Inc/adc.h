/*
 * adc.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

extern uint16_t *const adc_reg_addr;

void adc_set_reference_voltage(float voltage);
float adc_get_reference_voltage(void);
uint8_t adc_get_num_of_channels(void);
float adc_get_voltage(const uint8_t channel);

#endif /* INC_ADC_H_ */
