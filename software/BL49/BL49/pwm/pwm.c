/*
 * pwm.c
 *
 * Created: 22.05.2022 21:21:32
 *  Author: Heinrich
 */ 

#include "pwm.h"

void pwm_init (void)
{
	// pin pc1 (OC1B, Timer 1 output compare B) connected to sensor heater
	// fast pwm mode, prescaler 64,  top 0x03ff (1023) = 244Hz
	// pwm mode 7, 10-bit resolution
	// clear oc1b on compare match
		
	TCCR1A |= (1 << WGM11)|(1 << WGM10)|(1 << COM1B1);
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
	pwm_setDuty(0);
}

void pwm_setDuty (uint16_t duty)
{
	if (duty == 0)
	{
		TCCR1A &= (1 << COM1B1);
	}
	else
	{
		OCR1B = duty;
		if (!(TCCR1A |= (1 << COM1B1)))
		{
			TCCR1A |= (1 << COM1B1);
		}
	}
}

void pwm_shutdown (void)
 {
	 TCCR1A &= (1 << COM1B1);
 }