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
	STATUS_OKAY,
	STATUS_E_NOPOWER,
	STATUS_E_NOSENSOR,
	CJ125_E_SHORTCIRCUITGND,
	CJ125_E_SHORTCIRCUITBAT
} tcj125_status;

uint16_t cj125_readSignature (void);
tcj125_status cj125_readStatus (void);



#endif /* CJ125_H_ */