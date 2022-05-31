/*
 * adc.h
 *
 * Created: 22.05.2022 20:42:55
 *  Author: Heinrich
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>

void adc_init (void);
uint16_t adc_read_UA (void);
uint16_t adc_read_UR (void);
uint16_t adc_read_reference (void);
uint16_t adc_read_battery (void);

#endif /* ADC_H_ */