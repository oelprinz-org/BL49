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
} tSensorStatus;

typedef enum
{
	OK			= 0x0
	, FAULT		= 0x1
} tSensorFaultState;

typedef enum
{
	Reserved1		= 0x1
	, BOSCH_LSU49	= 0x2
	, Reserved2		= 0x4
	, Reserved3		= 0x8	
} tSensorDetectedStatus;

typedef struct 
{
	uint16_t Shunt;
	uint8_t Amplification;
	uint16_t Ua_ref;
	uint16_t Ur_ref;
	uint16_t Ur_ref_raw;
	uint16_t Ua;
	uint16_t Ur;
	int16_t Ip;
	uint16_t Lambda;
	uint16_t O2;
	uint16_t HeaterVoltage;
	uint16_t SystemVoltage;
	bool SystemVoltageOK;
	uint8_t diagRegister;
	tSensorStatus SensorStatus;
	tSensorFaultState SensorFaultState;
	tSensorDetectedStatus SensorDetectedStatus;
} tSensor;

typedef struct 
{
	float pGain;
	float iGain;
	float dGain;
	int16_t iState;
	int16_t dState;
	int16_t iMin;
	int16_t iMax;
	int16_t pwmMin;
	int16_t pwmMax;
} tPIDController;

extern uint16_t adc_read_battery (void);

void sensor_init (tSensor *sensor, uint8_t amplification_factor);
void sensor_update_ua (tSensor *sensor, uint16_t ua_millis);
void sensor_update_ur (tSensor *sensor, uint16_t ur_millis);

// heater stuff...
void heater_init (void);
void heater_setDuty (uint16_t duty);
void heater_setVoltage (uint16_t voltageMillis);
void sensor_update_status (void);
uint16_t calc_pid (uint16_t referenceValue, uint16_t measuredValue, bool inverted);


#endif /* SENSOR_H_ */