/*
 * board.c
 *
 * Created: 23.05.2022 16:29:43
 *  Author: Heinrich
 */ 

#include "board.h"
#include "../helpers.h"
// #include "../adc/adc.h"

tBoard board;

void board_init (tBoard *board)
{
	board->input1_state = LOW;
	board->input2_state = LOW;
	
	// init the both inputs, in1 = pc6, in2 = pb4
	DDRC  &= ~(1 << PINC6);
	DDRB  &= ~(1 << PINB4);
	
	// init LED2 (pb5) and LED2 (pb6)
	// led1 is power
	DDRB |= (1 << PINB5)|(1 << PINB6);
	// switch them on
	PORTB |= (1 << PINB5)|(1 << PINB6);
}

void board_read_inputs (tBoard *board)
{
	// is pinc6 high?
	if ((PINC & (1 << PINC6)) == 1)
	{
		board->input1_state = HIGH;
	}
	else
	{
		board->input1_state = LOW;
	}
	
	// is pinb4 high?
	if ((PINB & (1 << PINB4)) == 1)
	{
		board->input2_state = HIGH;
	}
	else
	{
		board->input2_state = LOW;
	}
}

bool isEnabled (void)
{
	return (bool) ((PINC & (1 << PINC6)) == 1);
}