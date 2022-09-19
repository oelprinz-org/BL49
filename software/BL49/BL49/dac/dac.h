/*
 * dac.h
 *
 * Created: 22.05.2022 22:20:16
 *  Author: Heinrich
 */ 


#ifndef DAC_H_
#define DAC_H_

#include <avr/io.h>
#include <stdint.h>
#include "../helpers.h"

void dac_init (void);
uint16_t calculate_dac_value (uint16_t lambda_millis);
void dac_setValue(uint16_t value);
void dac_update_output (uint16_t lambda_millis);

#endif /* DAC_H_ */