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
float adc_reg_to_voltage(uint32_t value);

#endif /* INC_ADC_H_ */
