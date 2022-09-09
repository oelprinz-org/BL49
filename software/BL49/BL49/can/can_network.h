/*
 * can_network.h
 *
 * Created: 01.06.2022 17:20:59
 *  Author: Heinrich
 */ 


#ifndef CAN_NETWORK_H_
#define CAN_NETWORK_H_

#include "can_lib.h"
#include "../sensor/sensor.h"

typedef union
{
	struct  
	{
		uint8_t SensorDetectedStatus : 4;
		uint8_t unused1 : 1;
		uint8_t UsingFreeAirCal : 1;
		uint8_t unused2 : 1;
		uint8_t DataValidState : 1;	
	} signals;
	uint8_t raw_byte;
} T_BYTE6_AEM;

typedef union
{
	struct  
	{
		uint8_t SensorStatus : 5;
		uint8_t unused1 : 1;
		uint8_t SensorFaultState : 1;
		uint8_t unused2 : 1;
	} signals;
	uint8_t raw_byte;
} T_BYTE7_AEM;


void can_network_init (uint8_t mode);
void can_send_aem_message(tSensor sensor);
void can_send_debug_message(uint16_t ur_ref_raw, uint16_t ur_raw, uint8_t pid, uint8_t signature, uint8_t diagRegister);


#endif /* CAN_NETWORK_H_ */