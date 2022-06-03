/*
 * can_network.c
 *
 * Created: 01.06.2022 17:21:56
 *  Author: Heinrich
 */ 

#include "can_network.h"
#include "../helpers.h"

/*
	There are three different types of CAN modules available:
	-> 2.0A - Considers 29 bit ID as an error
	-> 2.0B Passive - Ignores 29 bit ID messages
	-> 2.0B Active - Handles both 11 and 29 bit ID Messages
*/

void can_network_init (uint8_t mode)
{
	can_init(mode);
}

void can_send_aem_message(tSensor sensor, uint16_t vBatt)
{
	st_cmd_t aem_message;
	
	T_BYTE6_AEM byte6;
	T_BYTE7_AEM byte7;	
	
	aem_message.id.ext = 0x180;
	aem_message.ctrl.ide = 1;
	aem_message.ctrl.rtr = 0;
	aem_message.dlc = 8;
	aem_message.cmd = CMD_TX_DATA;
	uint8_t aem_pt_data[aem_message.dlc];
	
	
	aem_pt_data[0] = high(sensor.Lambda*10);
	aem_pt_data[1] = low(sensor.Lambda*10);

	aem_pt_data[2] = high(sensor.O2);
	aem_pt_data[3] = low(sensor.O2);

	aem_pt_data[4] = (vBatt / 100);
	
	aem_pt_data[5] = (vBatt / 100)-20;
	
	byte6.signals.SensorDetectedStatus = sensor.DetectedStatus;
	byte6.signals.DataValidState = 0x1;
	byte6.signals.UsingFreeAirCal = 0x0;
	aem_pt_data[6] = byte6.raw_byte;
	
	byte7.signals.SensorFaultState = sensor.Status == ERROR ? 0x1 : 0x00;
	byte7.signals.SensorStatus = sensor.Status;
	aem_pt_data[7] = byte7.raw_byte;
	
	aem_message.pt_data = &aem_pt_data[0];
	
	while(can_cmd(&aem_message) != CAN_CMD_ACCEPTED);					// wait for MOb to configure
	while(can_get_status(&aem_message) == CAN_STATUS_NOT_COMPLETED);	// wait for a transmit request to come in, and send a response
}