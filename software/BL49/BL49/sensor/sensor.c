/*
 * sensor.c
 *
 * Created: 23.05.2022 15:12:39
 *  Author: Heinrich
 */ 

#include "sensor.h"
#include "../board/board.h"
#include "../helpers.h"

// pump current and lambda values from Bosch datasheet
// Ip * 1000
// lambda * 1000
int16_t ip_values[] = {-2000, -1602, -1243, -927, -800, -652, -405, -183, -106, -40, 0, 15, 97, 193, 250, 329, 671, 938, 1150, 1385, 1700, 2000, 2150, 2250};
uint16_t lambda_values[] = {650, 700, 750, 800, 822, 850, 900, 950, 970, 990, 1003, 1010, 1050, 1100, 1132, 1179, 1429, 1701, 1990, 2434, 3413, 5391, 7506, 10119};
	
// pump current corresponding to O2 concentration
// ip_values_o2 * 1000
// o2_values_percent * 1000, ready to transfer per CAN
int16_t ip_values_o2[] = {0, 330, 670, 940, 1380, 2540};
uint16_t o2_values_percent[] = {0, 3000, 6000, 8290, 12000, 20950};

tSensor sensor1;
extern tBoard board;

//PID regulation variables.
int dState;                                                         /* Last position input. */
int iState;                                                         /* Integrator state. */
const int iMax = 250;                                               /* Maximum allowable integrator state. */
const int iMin = -250;                                              /* Minimum allowable integrator state. */
const float pGain = 120;                                            /* Proportional gain. Default = 120*/
const float iGain = 0.8;                                            /* Integral gain. Default = 0.8*/
const float dGain = 10;                                             /* Derivative gain. Default = 10*/

const uint16_t pwmMin = 0;											/* we have 8 bit fast mode PWM, min value*/
const uint16_t pwmMax = 255;										/* we have 8 bit fast mode PWM, max value*/

void sensor_init (tSensor *sensor, uint8_t amplification_factor)
{
	sensor->Status = RESET;
	sensor->Ip = 0;
	sensor->Lambda = 0;
	sensor->Ua = 0;
	sensor->Ua_ref = 0;
	sensor->Ur = 0;
	sensor->Ur_ref = 0;
	sensor->O2 = 0;
	sensor->HeaterVoltage = 0;
	sensor->DetectedStatus = BOSCH_LSU49;
	sensor->Amplification = amplification_factor;
	
	heater_init();
}

tSensorStatus sensor_get_diag_status (void)
{
	uint8_t diagReg;
	tSensorStatus status;
	
	
	if (cj125_readStatus(&diagReg) == COMMAND_VALID)
	{
		// check that everythingis okay...
		if (diagReg == CJ125_DIAG_REG_STATUS_OK)
		{
			return SENSOR_OKAY;
		}
		
		// if not, check what's wrong.
		// check the heater
		switch ((diagReg >> 6)&CJ125_DIAG_MASK)
		{
			case 0:
			case 2:
				status = HEATER_SHORT_CIRCUIT;
				break;
				
			case 1:
				status = HEATER_OPEN_CIRCUIT;
				break;
			case 3:
				status = SENSOR_OKAY;
				break;
		}
	}
	else
	{
		status = ERROR;
	}
	
	return status;
}

int16_t calculate_ip (uint16_t Ua_ref, uint16_t Ua, uint8_t amp)
{
	int16_t ip;
	int32_t delta;
	int32_t divisor;
	// pump current calculation formula: Ip = (((Ua - Ua_ref) * PUMP_FACTOR ) / (SENSOR_SHUT * AMPLIFICATION)) * 1000
	delta = ((int32_t)Ua - (int32_t)Ua_ref) * 1000;
	divisor = (int32_t)SENSOR_SHUNT * (int32_t)amp;	
	ip = (int16_t)(((float)delta / (float)divisor)*1000);
	return ip;
}

uint16_t calculate_lambda (int16_t Ip)
{
	uint8_t counter = 0;
	uint16_t Lambda = 0;
	
	float gain, offset;
	
	// check against the both ends
	if (Ip <= ip_values[0])
	{
		return lambda_values[0];
	}
	
	if (Ip >= ip_values[23])
	{
		return lambda_values[23];
	}
	
	while ((Lambda == 0) && (counter < 23))
	{
		// exists an exact value?
		if ( ip_values[counter] == Ip)
		{
			Lambda = lambda_values[counter];
		}
		
		// ip value is between two values
		else if ((ip_values[counter] <= Ip) && (Ip <= ip_values[counter + 1]))
		{
			gain = (float) (lambda_values[counter+1] - lambda_values[counter]) / (ip_values[counter + 1] -  ip_values[counter]);
			
			offset = (lambda_values[counter+1] - (ip_values[counter + 1] * gain));
			
			Lambda = (uint16_t) ((Ip * gain) + offset);
		}
		counter++;
	}
	return Lambda;
}

uint16_t calculate_o2(int16_t Ip)
{
	uint8_t counter = 0;
	uint16_t o2 = 0;

	float gain, offset;

	// check against the both ends
	if (Ip <= ip_values_o2[0])
	{
		return o2_values_percent[0];
	}

	if (Ip >= ip_values_o2[5])
	{
		return o2_values_percent[5];
	}

	while ((o2 == 0) && (counter < 5))
	{
		// exists an exact value?
		if (ip_values_o2[counter] == Ip)
		{
			o2 = o2_values_percent[counter];
		}

		// ip value is between two values
		else if ((ip_values_o2[counter] <= Ip) && (Ip <= ip_values_o2[counter + 1]))
		{
			gain = (float)(o2_values_percent[counter + 1] - o2_values_percent[counter]) / (ip_values_o2[counter + 1] - ip_values_o2[counter]);

			offset = (o2_values_percent[counter + 1] - (ip_values_o2[counter + 1] * gain));

			o2 = (uint16_t)((Ip * gain) + offset);
		}
		counter++;
	}
	return o2;
}

void sensor_update_ua (tSensor *sensor, uint16_t ua_millis)
{
	sensor->Ua = ua_millis;
	sensor->Ip = calculate_ip (sensor->Ua_ref, sensor->Ua, sensor->Amplification);
	sensor->Lambda = calculate_lambda(sensor->Ip);
	sensor->O2 = calculate_o2(sensor->Ip);
}

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
	// TCCR1A |= (1 << WGM11)|(1 << WGM10)|(1 << COM1B1);
	// TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
	
	// pin pc1 (OC1B, Timer 1 output compare B) connected to sensor heater
	// fast pwm mode, prescaler 256,  top 0x0ff (255) = 244Hz
	// pwm mode 5, 8-bit resolution
	// clear oc1b on compare match
	TCCR1A |= (1 << WGM10)|(1 << COM1B1);
	TCCR1B |= (1 << WGM12)|(1 << CS12);
	
	heater_setDuty(0);
}

void heater_setVoltage (uint16_t voltageMillis)
{
	uint16_t duty = 0;
	sensor1.HeaterVoltage = voltageMillis;
	
	if (voltageMillis == 0)
	{
		heater_shutdown();
	}
	else
	{
		duty = voltage2duty_cycle (voltageMillis, board.vBatt, 256);
		heater_setDuty(duty);	
	}
}

void heater_setDuty (uint16_t duty)
{
	if (duty == 0)
	{
		
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
	sensor1.HeaterVoltage = 0;
}

uint16_t heater_pid_control (uint16_t Ur, uint16_t Ur_calibration)
{
	//Calculate error term.
	int16_t error = (int16_t)Ur_calibration - (int16_t)Ur;
	
	//Set current position.
	int16_t position = (int16_t)Ur;
	
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