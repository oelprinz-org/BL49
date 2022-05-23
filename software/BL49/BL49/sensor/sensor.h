/*
 * sensor.h
 *
 * Created: 23.05.2022 15:12:48
 *  Author: Heinrich
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

#define SENSOR_SHUNT	61.9

typedef struct {
	uint16_t Ua_ref;
	uint16_t Ur_ref;
	uint16_t Ua;
	uint16_t Ur;	
	int16_t Ip;
} tSensor;

void calculate_ip (void);
uint16_t calculate_lambda (int16_t ip);

#endif /* SENSOR_H_ */