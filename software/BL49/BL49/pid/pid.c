/*
 * pid.c
 *
 * Created: 05.12.2021 20:49:54
 *  Author: Heinrich
 */ 

#include "pid.h"

uint16_t pid_control (uint16_t Ur, uint16_t Ur_calibration)
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