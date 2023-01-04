/*
 * timer.c
 *
 * Created: 23.05.2022 16:46:03
 *  Author: Heinrich
 */ 

#include "timer.h"
#include <avr/interrupt.h>

volatile uint32_t ms_counter;
volatile uint8_t timer_10ms;
volatile uint8_t timer_20ms;
volatile uint8_t timer_50ms;
volatile uint8_t timer_100ms;
volatile uint8_t timer_250ms;
volatile uint16_t timer_500ms;
volatile uint16_t timer_1000ms;

void init_1ms_timer (void)
{
	// timer between interrupts: 0.001s
	// Frequency: 1000Hz
	// F_CPU: 16MHz
	// Prescaler: 64
	// Compare match register: 64
	ms_counter = 0;
	timer_10ms = 0;
	timer_20ms = 0;
	timer_50ms = 0;
	timer_100ms = 0;
	timer_250ms = 0;
	timer_500ms = 0;
	timer_1000ms = 0;
	TIMER_TASKS = 0;

	OCR0A = 249;
	TIMSK0 |= (1 << OCIE0A);			// output compare A interrupt
	// TCCR0A |= (1 << COM0A0);			// clear OC0A on compare match
	TCCR0A |= (1<<WGM01);			// Mode = CTC
	TCCR0B |= (1 << CS01)|(1 << CS00);	// prescaler 64
}

void timer_delay_ms (uint16_t millis)
{
	do 
	{
		OCR0A = 249;
		TCCR0A |= (1<<WGM01);				// Mode = CTC
		TCCR0B |= (1 << CS01)|(1 << CS00);	// prescaler 64
		while ((TIFR1 & (1 << OCF1A)) == 0);
		TIFR1 |= (1 << OCF1A);
		millis--;
	} while (millis > 0);
}

ISR (TIMER0_COMPA_vect)
{
	ms_counter++;
	timer_10ms++;
	timer_20ms++;
	timer_50ms++;
	timer_100ms++;
	timer_250ms++;
	timer_500ms++;
	timer_1000ms++;
	
	if (timer_10ms == 10)
	{
		timer_10ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_10ms);
	}
	
	if (timer_20ms == 20)
	{
		timer_20ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_20ms);
	}
	
	if (timer_50ms == 50)
	{
		timer_50ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_50ms);
	}
	
	if (timer_100ms == 100)
	{
		timer_100ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_100ms);
	}
	
	if (timer_250ms == 250)
	{
		timer_250ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_250ms);
	}
	
	if (timer_500ms == 500)
	{
		timer_500ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_500ms);
	}
	
	if (timer_1000ms == 1000)
	{
		timer_1000ms = 0;
		bit_set(TIMER_TASKS, BIT_TIMER_1000ms);
	}
}