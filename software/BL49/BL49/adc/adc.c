/*
 * adc.c
 *
 * Created: 22.05.2022 20:42:33
 *  Author: Heinrich
 */ 

#include "adc.h"

void adc_init (void)
{
	// avcc with cap on aref
	ADCSRB |= (1 << AREFEN);
	ADMUX = (1 << REFS0);
		
	// enable adc, prescalee 128 (125kHz)
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)|(1 << ADEN);
}

uint16_t adc_read (uint8_t channel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= (1<<ADSC);		// start single conversion
	while(ADCSRA & (1<<ADSC));	// wait till done
	return ADC;
}

uint16_t adc_readAvg (uint8_t channel, uint8_t samples)
{
	uint32_t buf = 0;
	
	for (uint8_t i = 0; i < samples; i++)
	{
		buf += adc_read(channel);
	}
	return (uint16_t) (buf / samples);
}

uint16_t adc_read_UA (void)
{
	return adc_readAvg (8, 5);
}

uint16_t adc_read_UR (void)
{
	return adc_readAvg (9, 5);
}

uint16_t adc_read_reference (void)
{
	return adc_readAvg (3, 5);
}

uint16_t adc_read_battery (void)
{
	return adc_readAvg (5, 5);
}