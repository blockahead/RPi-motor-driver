/*
 * adc.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

void adc_set_reference_voltage(float voltage);
float adc_get_reference_voltage(void);
float adc_get_voltage(uint16_t reg_value);

#endif /* INC_ADC_H_ */
