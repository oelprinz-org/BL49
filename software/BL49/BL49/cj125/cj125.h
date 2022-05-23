/*
 * cj125.h
 *
 * Created: 22.05.2022 23:45:57
 *  Author: Heinrich
 */ 


#ifndef CJ125_H_
#define CJ125_H_

#include <avr/io.h>
#include "../spi/spi.h"

typedef enum
{
	STATUS_ERROR,
	STATUS_OKAY,
	STATUS_E_NOPOWER,
	STATUS_E_NOSENSOR,
	CJ125_E_SHORTCIRCUITGND,
	CJ125_E_SHORTCIRCUITBAT
} tcj125_status;

uint8_t cj125_readSignature (void);
tcj125_status cj125_readStatus (void);
void cj125_set_calibration_mode (void);
void cj125_set_running_mode_v8 (void);
void cj125_set_running_mode_v17 (void);


#endif /* CJ125_H_ */