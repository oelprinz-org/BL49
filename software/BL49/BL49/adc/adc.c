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
	ADCSRA |= (1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
}

uint16_t adc_read (uint8_t channel)
{
	ADMUX |= (ADMUX & ~(0x1F)) | (channel & 0x1F);			// select channel
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