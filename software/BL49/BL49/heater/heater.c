/*
 * pwm.c
 *
 * Created: 22.05.2022 21:21:32
 *  Author: Heinrich
 */ 

#include "heater.h"

//PID regulation variables.
int dState;                                                         /* Last position input. */
int iState;                                                         /* Integrator state. */
const int iMax = 250;                                               /* Maximum allowable integrator state. */
const int iMin = -250;                                              /* Minimum allowable integrator state. */
const float pGain = 120;                                            /* Proportional gain. Default = 120*/
const float iGain = 0.8;                                            /* Integral gain. Default = 0.8*/
const float dGain = 10;                                             /* Derivative gain. Default = 10*/

const uint16_t pwmMin = 0;											/* we have 10 bit fast mode PWM, min value*/
const uint16_t pwmMax = 1023;										/* we have 10 bit fast mode PWM, max value*/

void heater_init (void)
{
	// init COM1B1 as GPIO (PC1) and default low:
	// if bit COM1B1 is 0, then pc1 acts as gpio with original state (so, output and low)
	DDRC |= (1 << PINC1);		// PC1 is an output
	PORTC &= ~(1 << PINC1);	// and is low
	
	// pin pc1 (OC1B, Timer 1 output compare B) connected to sensor heater
	// fast pwm mode, prescaler 64,  top 0x03ff (1023) = 244Hz
	// pwm mode 7, 10-bit resolution
	// clear oc1b on compare match
	TCCR1A |= (1 << WGM11)|(1 << WGM10)|(1 << COM1B1);
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
	heater_setDuty(0);
}

void heater_setDuty (uint16_t duty)
{
	if (duty == 0)
	{
		TCCR1A &= ~(1 << COM1B1);
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

void heater_shutdown (void)
{
	TCCR1A &= ~(1 << COM1B1);
}

uint16_t heater_pid_control (uint16_t Ur, uint16_t Ur_calibration)
{
	//Calculate error term.
	uint16_t error = Ur_calibration - Ur;
	
	//Set current position.
	uint16_t position = Ur;
	
	//Calculate proportional term.
	float pTerm = -pGain * error;
	
	//Calculate the integral state with appropriate limiting.
	iState += error;
	
	if (iState > iMax) iState = iMax;
	if (iState < iMin) iState = iMin;
	
	//Calculate the integral term.
	float iTerm = -iGain * iState;
	
	//Calculate the derivative term.
	float dTerm = -dGain * (dState - position);
	dState = position;
	
	//Calculate regulation (PI).
	uint16_t RegulationOutput = pTerm + iTerm + dTerm;
	
	//Set maximum heater output (full power).
	if (RegulationOutput > pwmMax) RegulationOutput = pwmMax;
	
	//Set minimum heater value (cooling).
	if (RegulationOutput < pwmMin) RegulationOutput = pwmMin;

	//Return calculated PWM output.
	return RegulationOutput;
}