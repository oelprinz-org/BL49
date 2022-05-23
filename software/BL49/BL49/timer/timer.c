/*
 * timer.c
 *
 * Created: 23.05.2022 16:46:03
 *  Author: Heinrich
 */ 

#include "timer.h"
#include <avr/interrupt.h>

volatile uint16_t counter;

void init_1ms_timer (void)
{
	counter = 0;
	TIMSK0 |= (1 << TOIE0);
	TCCR0A = 0;
	TCCR0B |= (1 << CS02)|(1 << CS00);
	
}

ISR (TIMER0_OVF_vect)
{
	counter++;
}