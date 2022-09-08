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

tPIDController pidController;
tSensor sensor1;
extern tBoard board;

void sensor_init (tSensor *sensor, uint8_t amplification_factor)
{
	sensor->SensorStatus = RESET;
	sensor->SensorFaultState = OK;
	sensor->Shunt = 61900;
	sensor->Ip = 0;
	sensor->Lambda = 0;
	sensor->Ua = 0;
	sensor->Ua_ref = 0;
	sensor->Ur = 0;
	sensor->Ur_ref = 0;
	sensor->O2 = 0;
	sensor->HeaterVoltage = 0;
	sensor->SensorDetectedStatus = BOSCH_LSU49;
	sensor->Amplification = amplification_factor;
	
	heater_init();
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
	
	// init pid controller...
	// original values: p = 120; i = 0.8; d = 10;
	
	pidController.pGain = 3.1;
	pidController.iGain = 0;
	pidController.dGain = 0;
	
	pidController.iMin = -250;
	pidController.iMax = 250;
	pidController.pwmMin = 0;
	pidController.pwmMax = 240;
		
	heater_setDuty(0);
}

void sensor_update_status (void)
{
	uint8_t diagReg;
	
	
	if (cj125_readStatus(&diagReg) == COMMAND_VALID)
	{
		// check that everythingis okay...
		if (diagReg == CJ125_DIAG_REG_STATUS_OK)
		{
			sensor1.SensorFaultState = OK;
		}
		else
		{	
			sensor1.SensorFaultState = FAULT;
			// if not, check what's wrong.
			// check the heater
			switch ((diagReg >> 6)&CJ125_DIAG_MASK)
			{
				case 0:
				case 2:
					sensor1.SensorStatus = HEATER_SHORT_CIRCUIT;
					break;
				
				case 1:
					sensor1.SensorStatus = HEATER_OPEN_CIRCUIT;
					break;
			}
		}
	}
	else
	{
		sensor1.SensorStatus = ERROR;
		sensor1.SensorFaultState = FAULT;
	}
}

int16_t calculate_ip (uint16_t Ua_ref, uint16_t Ua, uint8_t amp)
{
	int16_t ip;
	int32_t delta;
	int32_t divisor;
	// pump current calculation formula: Ip = (((Ua - Ua_ref) * PUMP_FACTOR ) / (SENSOR_SHUT * AMPLIFICATION)) * 1000
	delta = ((int32_t)Ua - (int32_t)Ua_ref) * 1000;
	// divisor = (int32_t)SENSOR_SHUNT * (int32_t)amp;	
	divisor = (int32_t)sensor1.Shunt * (int32_t)amp;	
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
		Lambda = lambda_values[0];
	}
	
	if (Ip >= ip_values[22])
	{
		Lambda = lambda_values[22];
	}
	
	while ((Lambda == 0) && (counter < 22))
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
	
	if (Lambda > 6600)
	{
		Lambda = 6553;
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

void sensor_update_ur (tSensor *sensor, uint16_t ur_millis)
{
	sensor->Ur = ur_millis;
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

uint16_t calc_pid (uint16_t referenceValue, uint16_t measuredValue, bool inverted)
{
	float pTerm = 0, iTerm = 0, dTerm = 0;
	// calculation error:
	int16_t error = (int16_t) referenceValue - (int16_t) measuredValue;
	int16_t position = (int16_t) measuredValue;
	
	// calculate p-term;
	if (inverted)
	{
		pTerm = -pidController.pGain * error;
	} else {
		pTerm = pidController.pGain * error;
	}
	
	
	//Calculate the integral state
	pidController.iState += error;
	
	// check limits of iState
	if (pidController.iState > pidController.iMax) pidController.iState = pidController.iMax;
	if (pidController.iState < pidController.iMin) pidController.iState = pidController.iMin;
	
	//Calculate the integral term.
	if (inverted)
	{
		iTerm = -pidController.iGain * pidController.iState;
	} else {
		iTerm = pidController.iGain * pidController.iState;
	}
	
	//Calculate the derivative term.
	if (inverted)
	{
		dTerm = -pidController.dGain * (pidController.dState - position);
	} else {
		dTerm = pidController.dGain * (pidController.dState - position);
	}
	pidController.dState = position;
	
	//Calculate regulation (PI).
	int16_t RegulationOutput = pTerm + iTerm + dTerm;
	
	// check limits of pwm here....
	
	if (RegulationOutput > pidController.pwmMax) 
	{
		RegulationOutput = pidController.pwmMax;
	}
	
	if (RegulationOutput < pidController.pwmMin)
	{
		 RegulationOutput = pidController.pwmMin;
	}
		
	return  (uint16_t) RegulationOutput;
}