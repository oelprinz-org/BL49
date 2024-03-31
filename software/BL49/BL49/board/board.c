/*
 * board.c
 *
 * Created: 23.05.2022 16:29:43
 *  Author: Heinrich
 */ 

#include "board.h"
#include "../helpers.h"

tBoard board;

void board_init (tBoard *board)
{
	board->input1_state = LOW;
	board->input2_state = LOW;
	
	// init the both inputs, in1 = pc6, in2 = pb4
	DDRC  &= ~(1 << PINC6);
	DDRB  &= ~(1 << PINB4);
	
	// activate internal pull-ups, since the inputs are "active low"
	PORTC |= (1 << PINC6);
	PORTB |= (1 << PINB4);
	
	// init LED2 (pb5) and LED2 (pb6)
	// led1 is power
	DDRB |= (1 << PINB5)|(1 << PINB6);
	
	// switch them off
	// PORTB |= (1 << PINB5)|(1 << PINB6);
	led1_setState(OFF);
	led2_setState(OFF);
}

void board_read_inputs (tBoard *board)
{	
	// bit_is_set from sfr_def.h
	// is pinc6 high?
	
	if (bit_is_set(PINC, PINC6))
	{
		board->input1_state = HIGH;
	}
	else
	{
		board->input1_state = LOW;
	}
	
	// is pinb4 high?
	if (bit_is_set(PINB,PINB4))
	{
		board->input2_state = HIGH;
	}
	else
	{
		board->input2_state = LOW;
	}
}

// input 1 is "active low", so we check for LOW state of this input!
bool isActive (void)
{
	return (bool) (bit_is_clear(PINC, PINC6));
}

void led1_toggle (void)
{
	PORTB ^= (1 << PINB5);
}

void led2_toggle (void)
{
	PORTB ^= (1 << PINB6);
}

void led1_setState (tLedState state)
{
	switch (state)
	{
		case ON:
		PORTB |= (1 << PINB5);
		break;
		case OFF:
		PORTB &= ~(1 << PINB5);
		break;
	}
}

void led2_setState (tLedState state)
{
	switch (state)
	{
		case ON:
		PORTB |= (1 << PINB6);
		break;
		case OFF:
		PORTB &= ~(1 << PINB6);
		break;
	}
}