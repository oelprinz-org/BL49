/*
 * sensor.h
 *
 * Created: 23.05.2022 15:12:48
 *  Author: Heinrich
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>
#include <avr/io.h>
#include "../cj125/cj125.h"
#define SENSOR_SHUNT	61900

typedef enum
{
	RESET							= 0x0
	, WARMING_UP					= 0x1
	, STABILIZING					= 0x2
	, READING_NERST_PUMP_IMPEDANCE	= 0x3
	, EQUALIZING					= 0x4
	, READING_RCAL_VALUE			= 0x5
	, RUN							= 0x6
	, OVERHEAT						= 0x7
	, OVERCOOL						= 0x8
	, HEATER_SHORT_CIRCUIT			= 0x9
	, HEATER_OPEN_CIRCUIT			= 0xA
	, START_FAC						= 0xB	// FAC: FreeAirCalibration: since we are using LSU4.9 it is not necessary
	, PERFORMING_FAC				= 0xC	// FAC: FreeAirCalibration: since we are using LSU4.9 it is not necessary
	, DETECTING_SENSOR				= 0xD
	, READING_JUNCTION_IMPEDANCE	= 0xE
	, EVAP_START_UP					= 0xF
	, DETERMINING_SENSOR_TYPE		= 0x10
	, PREPARING_TO_RUN				= 0x11
	, SAVE_SENSOR					= 0x12
	, NEED_NEW_FAC					= 0x13	// FAC: FreeAirCalibration: since we are using LSU4.9 it is not necessary
	, ERROR							= 0x14
	, SENSOR_OKAY					= 0x15
} tSensorStatus;

typedef enum
{
	Reserved1		= 0x1
	, BOSCH_LSU49	= 0x2
	, Reserved2		= 0x4
	, Reserved3		= 0x8	
} tSensorDetectedStatus;

typedef struct {
	uint16_t Shunt;
	uint8_t Amplification;
	uint16_t Ua_ref;
	uint16_t Ur_ref;
	uint16_t Ua;
	uint16_t Ur;
	int16_t Ip;
	uint16_t Lambda;
	uint16_t O2;
	uint16_t HeaterVoltage;
	tSensorStatus Status;
	tSensorDetectedStatus DetectedStatus;
} tSensor;

void sensor_init (tSensor *sensor, uint8_t amplification_factor);
void sensor_update_ua (tSensor *sensor, uint16_t ua_millis);
// int16_t calculate_ip (uint16_t Ua_ref, uint16_t Ua, uint8_t amp);
// uint16_t calculate_lambda (int16_t Ip);
// uint16_t calculate_o2(int16_t Ip)

// heater stuff...
void heater_init (void);
void heater_setDuty (uint16_t duty);
void heater_shutdown (void);
uint16_t heater_pid_control (uint16_t Ur, uint16_t Ur_calibration);


#endif /* SENSOR_H_ */