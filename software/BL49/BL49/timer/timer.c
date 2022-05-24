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
	// timer between interrupts: 0.001s
	// Frequency: 1000Hz
	// F_CPU: 16MHz
	// Prescaler: 64
	// Compare match register: 64
	counter = 0;
	
	OCR0A = 249;
	TIMSK0 |= (1 << OCIE0A);			// output compare A interrupt
	// TCCR0A |= (1 << COM0A0);			// clear OC0A on compare match
	TCCR0A |= (1<<WGM01);			// Mode = CTC
	TCCR0B |= (1 << CS01)|(1 << CS00);	// prescaler 64
	
	
}

ISR (TIMER0_COMPA_vect)
{
	counter++;
}