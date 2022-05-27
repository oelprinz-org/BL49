/*
 * cj125.h
 *
 * Created: 22.05.2022 23:45:57
 *  Author: Heinrich
 */ 


#ifndef CJ125_H_
#define CJ125_H_

#include <avr/io.h>
#include "cj125_registers.h"
#include "../spi/spi.h"

typedef enum
{
	CJ125_COMMUNICATION_ERROR
	, CJ125_STATUS_ERROR
	, CJ125_STATUS_OKAY
	, CJ125_STATUS_E_NOPOWER
	, CJ125_STATUS_E_NOSENSOR
	, CJ125_E_SHORTCIRCUITGND
	, CJ125_E_SHORTCIRCUITBAT
} tcj125_status;

uint8_t cj125_readSignature (void);
tcj125_status cj125_readStatus (void);
void cj125_set_calibration_mode (void);
void cj125_set_running_mode_v8 (void);
void cj125_set_running_mode_v17 (void);
uint16_t cj125_read_init1_register (void);
uint16_t cj125_read_init2_register (void);

#endif /* CJ125_H_ */