/*
 * sensor.h
 *
 * Created: 23.05.2022 15:12:48
 *  Author: Heinrich
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

#define SENSOR_SHUNT	61900

typedef enum
{
	SENSOR_OFF
	, SENSOR_CALIBRATION
	, SENSOR_CONDENSATION
	, SENSOR_HEATING_UP
	, SENSOR_RUNNING
	, SENSOR_ERROR
} tSensorState;

typedef struct {
	uint16_t Shunt;
	uint8_t Amplification;
	uint16_t Ua_ref;
	uint16_t Ur_ref;
	uint16_t Ua;
	uint16_t Ur;
	int16_t Ip;
	uint16_t Lambda;
	tSensorState State;
} tSensor;

void sensor_init (tSensor *sensor, uint8_t amplification_factor);
void sensor_update_ua (tSensor *sensor, uint16_t ua_millis);
// int16_t calculate_ip (uint16_t Ua_ref, uint16_t Ua, uint8_t amp);
// uint16_t calculate_lambda (int16_t Ip);

#endif /* SENSOR_H_ */