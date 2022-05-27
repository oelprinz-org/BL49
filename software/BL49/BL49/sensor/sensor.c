/*
 * sensor.c
 *
 * Created: 23.05.2022 15:12:39
 *  Author: Heinrich
 */ 

#include "sensor.h"

// pump current and lambda values from Bosch datasheet
// Ip * 1000
// lambda * 1000
int16_t ip_values[] = {-2000, -1602, -1243, -927, -800, -652, -405, -183, -106, -40, 0, 15, 97, 193, 250, 329, 671, 938, 1150, 1385, 1700, 2000, 2150, 2250};
uint16_t lambda_values[] = {650, 700, 750, 800, 822, 850, 900, 950, 970, 990, 1003, 1010, 1050, 1100, 1132, 1179, 1429, 1701, 1990, 2434, 3413, 5391, 7506, 10119};

tSensor sensor1;

void sensor_init (tSensor *sensor, float shunt_resistance, uint8_t amplification_factor)
{
	sensor->State = SENSOR_OFF;
	sensor->Ip = 0;
	sensor->Lambda = 0;
	sensor->Ua = 0;
	sensor->Ua_ref = 0;
	sensor->Ur = 0;
	sensor->Ur_ref = 0;
	
	sensor->Amplification = amplification_factor;
	sensor->Shunt = (uint16_t) (shunt_resistance*1000);
}

int16_t calculate_ip (tSensor *sensor)
{
	int32_t delta;
	int32_t divisor;
	// pump current calculation formula: Ip = (((Ua - Ua_ref) * PUMP_FACTOR ) / (SENSOR_SHUT * AMPLIFICATION)) * 1000
	delta = ((int32_t)sensor->Ua - (int32_t)sensor->Ua_ref) * 1000;
	divisor = (int32_t)sensor->Shunt * (int32_t)sensor->Amplification;	
	sensor->Ip = (int16_t)(((float)delta / (float)divisor)*1000);
	return sensor->Ip;
}

uint16_t calculate_lambda (tSensor *sensor)
{
	uint8_t counter = 0;
	
	float gain, offset;
	
	// check against the both ends
	if (sensor->Ip <= ip_values[0])
	{
		sensor->Lambda = lambda_values[0];
		return sensor->Lambda;
	}
	
	if (sensor->Ip >= ip_values[23])
	{
		sensor->Lambda = lambda_values[23];
		return sensor->Lambda;
	}
	
	while ((sensor->Lambda == 0) && (counter < 23))
	{
		// exists an exact value?
		if ( ip_values[counter] == sensor->Ip)
		{
			sensor->Lambda = lambda_values[counter];
		}
		
		// ip value is between two values
		else if ((ip_values[counter] <= sensor->Ip) && (sensor->Ip <= ip_values[counter + 1]))
		{
			gain = (float) (lambda_values[counter+1] - lambda_values[counter]) / (ip_values[counter + 1] -  ip_values[counter]);
			
			offset = (lambda_values[counter+1] - (ip_values[counter + 1] * gain));
			
			sensor->Lambda = (uint16_t) ((sensor->Ip * gain) + offset);
		}
		counter++;
	}
	return sensor->Lambda;
}