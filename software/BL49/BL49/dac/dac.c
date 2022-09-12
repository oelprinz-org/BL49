/*
 * dac.c
 *
 * Created: 22.05.2022 22:20:03
 *  Author: Heinrich
 */ 

#include "dac.h"

/*
Lambda 0.65 --> 0.25
Lambda 1.30 --> 4.8

voltage = 7*lambda - 4.3
or
voltage_millis = 7 * lambda (with factor 1000, 650 for 0.65) - 4300
*/

// lambda factor: 1000
// voltage factor: 1000

uint16_t lambda_values_milli[] = {650, 700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, 1200, 1250, 1300};
uint16_t voltage_values_milli[] = {250, 600, 950, 1300, 1650, 2000, 2350, 2700, 3050, 3400, 3750, 4100, 4450, 4800};
uint16_t dac_values[] = {51, 123, 195, 266, 338, 410, 481, 553, 625, 696, 768, 840, 911, 983};

void dac_init (void)
{
	// dac output enable, dac enable, no autotrigger
	DACON = (1 << DAOE) |(1 << DAEN);
	DAC = 0;
}

void dac_setValue (uint16_t value)
{
	DAC = value;
}

uint16_t get_dac_value (uint16_t lambda_millis)
{
	uint16_t retVal = 0;
	uint8_t counter = 0;
	uint16_t voltage_millis = 0;
	
	// check agains ends:
	if (lambda_millis <= lambda_values_milli[0])
	{
		retVal = dac_values[0];
	}
	
	if ( lambda_millis >= lambda_values_milli[13])
	{
		retVal = dac_values[13];
	}
	
	
	while (retVal == 0 && counter < 14)
	{
		// do we have exact value?
		if (lambda_values_milli[counter] == lambda_millis)
		{
			retVal = dac_values[counter];
		}
		
		// are we between two values? if so, calculate
		else if (lambda_values_milli[counter] <= lambda_millis && lambda_millis <= lambda_values_milli[counter + 1])
		{
			// calculate voltage for given lambda:
			voltage_millis = lambda_millis * 7 - 4300;
			retVal = voltage2dac_millis(voltage_millis);	
		}
		counter++;
	}
	
	return retVal;	
}

void dac_update_output (uint16_t lambda_millis)
{
	uint16_t tmpValue = 0;
	
	tmpValue = get_dac_value(lambda_millis);
	dac_setValue(tmpValue);
}