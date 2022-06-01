/*
 * adc.c
 *
 * Created: 22.05.2022 20:42:33
 *  Author: Heinrich
 */ 

#include "adc.h"

// since on the channel 3 is a shut reference with 1225v connected,
// we can calculate the offset. 1225mV = ADC 251

int16_t v_reference_offset = 0;

void adc_init (void)
{
	// avcc with cap on aref
	ADCSRB |= (1 << AREFEN);
	ADMUX = (1 << REFS0);
	
	// enable adc, prescaler 8 (2MHz)
	ADCSRA |= (1 << ADPS1)|(1 << ADPS0)|(1 << ADEN);
	
	// Dummy readout to prevent further error readings
	ADCSRA |= (1 << ADSC);
	while ( ADCSRA & (1 << ADSC)){}
	(void) ADCW;
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
	return (uint16_t)((int16_t)adc_readAvg (8, 5) + v_reference_offset);
}

uint16_t adc_read_UR (void)
{
	return (uint16_t)((int16_t)adc_readAvg (9, 5) + v_reference_offset);
}

uint16_t adc_read_reference (void)
{
	uint16_t v_reference = 0;
	
	v_reference = adc_readAvg (3, 5);
	
	v_reference_offset = 251 - (int16_t) v_reference;
	
	return v_reference;
}

uint16_t adc_read_battery (void)
{
	return (uint16_t)((int16_t)adc_readAvg (5, 5) + v_reference_offset);
}