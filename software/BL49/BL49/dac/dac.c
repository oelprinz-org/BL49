/*
 * dac.c
 *
 * Created: 22.05.2022 22:20:03
 *  Author: Heinrich
 */ 

#include "dac.h"

void dac_init (void)
{
	// dac output enable, dac enable, no autotrigger
	DACON = (1 << DAOE) |(1 << DAEN);
}

void dac_setValue (uint16_t value)
{
	// DAC = 0xff;
	// DACL = low(value);
	// DACH = high(value);
	
	DAC = value;
}