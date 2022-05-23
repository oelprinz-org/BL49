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

void calculate_ip (void)
{
	sensor1.Ip = (((int16_t)sensor1.Ua - (int16_t)sensor1.Ua_ref) / (SENSOR_SHUNT * 8) * 1000);
}

uint16_t calculate_lambda (int16_t ip)
{
	uint16_t y = 0;
	uint8_t counter = 0;
	
	float gain, offset;
	
	// check against the both ends
	if (ip <= ip_values[0])
	{
		y = lambda_values[0];
	}
	
	if (ip >= ip_values[23])
	{
		y = lambda_values[23];
	}
	
	while ((y == 0) && (counter < 23))
	{
		// exists an exact value?
		if ( ip_values[counter] == ip)
		{
			y = lambda_values[counter];
		}
		
		// ip value is between two values
		else if ((ip_values[counter] <= ip) && (ip <= ip_values[counter + 1]))
		{
			gain = (float) (lambda_values[counter+1] - lambda_values[counter]) / (ip_values[counter + 1] -  ip_values[counter]);
			
			offset = (lambda_values[counter+1] - (ip_values[counter + 1] * gain));
			
			y = (uint16_t) ((ip * gain) + offset);
		}
		counter++;
	}
	return y;
}