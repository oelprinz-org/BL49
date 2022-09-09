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
	
	/*	
	st_cmd_t message;
	
	message.id.ext = 0x180;
	// message.ctrl.ide = 0;			// standard CAN frame type (2.0A)
	
	message.ctrl.ide = 1;				// we are using extended ID, check can_lib.c:118
	message.ctrl.rtr = 0;				// this message object is not requesting a remote node to transmit data back
	message.dlc = 1;
	message.cmd = CMD_TX_DATA;
	// message.pt_data = 0x01;
	
	while(can_cmd(&message) != CAN_CMD_ACCEPTED);					// wait for MOb to configure
	while(can_get_status(&message) == CAN_STATUS_NOT_COMPLETED);	// wait for a transmit request to come in, and send a response
	*/

void can_network_init (uint8_t mode)
{
	can_init(mode);
}

void can_send_aem_message(tSensor sensor)
{
	st_cmd_t aem_message;
	uint16_t tmpLambda;
	
	T_BYTE6_AEM byte6;
	T_BYTE7_AEM byte7;	
	
	aem_message.id.ext = 0x180;
	aem_message.ctrl.ide = 1;
	aem_message.ctrl.rtr = 0;
	aem_message.dlc = 8;
	aem_message.cmd = CMD_TX_DATA;
	uint8_t aem_pt_data[aem_message.dlc];
	
	tmpLambda = sensor.Lambda*10;
	aem_pt_data[0] = high(tmpLambda);
	aem_pt_data[1] = low(tmpLambda);

	aem_pt_data[2] = high(sensor.O2);
	aem_pt_data[3] = low(sensor.O2);

	aem_pt_data[4] = (sensor.SystemVoltage / 100);
	aem_pt_data[5] = (sensor.HeaterVoltage / 100);
	
	byte6.signals.SensorDetectedStatus = sensor.SensorDetectedStatus;
	byte6.signals.DataValidState = 0x1;
	byte6.signals.UsingFreeAirCal = 0x0;
	aem_pt_data[6] = byte6.raw_byte;
	
	byte7.signals.SensorFaultState = sensor.SensorFaultState;
	byte7.signals.SensorStatus = sensor.SensorStatus;
	aem_pt_data[7] = byte7.raw_byte;
	
	aem_message.pt_data = &aem_pt_data[0];
	
	while(can_cmd(&aem_message) != CAN_CMD_ACCEPTED);					// wait for MOb to configure
	while(can_get_status(&aem_message) == CAN_STATUS_NOT_COMPLETED);	// wait for a transmit request to come in, and send a response
}

void can_send_debug_message(uint16_t ur_ref_raw, uint16_t ur_raw, uint8_t pid, uint8_t signature, uint8_t diagRegister)
{
	st_cmd_t debug_message;
	debug_message.id.ext = 0x200;
	debug_message.ctrl.ide = 1;
	debug_message.ctrl.rtr = 0;
	debug_message.dlc = 7;
	debug_message.cmd = CMD_TX_DATA;
	uint8_t pt_data[debug_message.dlc];
	
	pt_data[0] = low(ur_ref_raw);
	pt_data[1] = high(ur_ref_raw);
	pt_data[2] = low(ur_raw);
	pt_data[3] = high(ur_raw);
	pt_data[4] = pid;
	pt_data[5] = signature;
	pt_data[6] = diagRegister;
	
	debug_message.pt_data = &pt_data[0];
	
	while(can_cmd(&debug_message) != CAN_CMD_ACCEPTED);					// wait for MOb to configure
	while(can_get_status(&debug_message) == CAN_STATUS_NOT_COMPLETED);	// wait for a transmit request to come in, and send a response
}