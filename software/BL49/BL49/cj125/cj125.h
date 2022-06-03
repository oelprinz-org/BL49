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
	COMMAND_VALID
	, COMMAND_NOT_VALID	
} tcj125_command_status;

typedef enum
{
	CALIBRATION
	, NORMAL_V8
	, NORMAL_V17
} tcj125_mode;

tcj125_command_status cj125_readSignature (uint8_t *sig);
tcj125_command_status cj125_readStatus (uint8_t *diagRegister);
tcj125_command_status cj125_set_calibration_mode (void);
tcj125_command_status cj125_set_running_mode_v8 (void);
tcj125_command_status cj125_set_running_mode_v17 (void);
tcj125_command_status cj125_get_mode (tcj125_mode *mode);

tcj125_command_status cj125_read_init1_register (uint16_t *reg);
tcj125_command_status cj125_read_init2_register (uint16_t *reg);

#endif /* CJ125_H_ */