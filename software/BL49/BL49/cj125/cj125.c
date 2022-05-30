/*
 * cj125.c
 *
 * Created: 22.05.2022 23:46:32
 *  Author: Heinrich
 */ 

#include "cj125.h"
#include "../helpers.h"

tcj125_command_status cj125_readSignature (uint8_t *sig)
{	
	uint16_t reg;
	
	reg = spi_read_write(CJ125_IDENT_REG_REQUEST);
	
	if (high(reg) == 0x28 || high(reg) == 0x2e)
	{
		*sig = low(reg);
		return COMMAND_VALID;
	}
	
	return COMMAND_NOT_VALID;
}

tcj125_command_status cj125_readStatus (tcj125_status *status)
{
	uint16_t statusReg = 0;
	
	statusReg = spi_read_write(CJ125_DIAG_REG_REQUEST);
	
	// depends of last operation (read or write) and depends of parity bit (0 or 1)
	// the high byte of status register can be 0x2e or 0x28, both values are ok.
	// the most important bit for a command is bit0, here is the status of current command
	// bit0 = 0 --> current command is valid
	// bit0 = 1 --> current command is not valid
	// so just check this bit...
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	// if the last command is valid, we check the content of low byte:
	
	switch(low(statusReg))
	{
		case CJ125_DIAG_REG_STATUS_OK:
		*status = CJ125_STATUS_OKAY;
		break;
		
		case CJ125_DIAG_REG_STATUS_NOPOWER:
		*status = CJ125_STATUS_E_NOPOWER;
		break;
		
		case CJ125_DIAG_REG_STATUS_NOSENSOR:
		*status = CJ125_STATUS_E_NOSENSOR;
		break;
		
		default:
		*status = CJ125_STATUS_ERROR;
		break;
	}
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_set_calibration_mode (void)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_INIT_REG1_MODE_CALIBRATE);
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	return COMMAND_VALID;
}

// amplification = 8, Lambda 0.65
tcj125_command_status cj125_set_running_mode_v8 (void)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_INIT_REG1_MODE_NORMAL_V8);
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	return COMMAND_VALID;
}

// amplification = 17, Lambda 0.75
tcj125_command_status cj125_set_running_mode_v17 (void)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_INIT_REG1_MODE_NORMAL_V17);
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_get_mode (tcj125_mode *mode)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_INIT_REG1_REQUEST);
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	switch (statusReg)
	{
		case CJ125_INIT_REG1_STATUS_CAL:
		*mode = CALIBRATION;
		break;
		
		case CJ125_INIT_REG1_STATUS_V8:
		*mode = NORMAL_V8;
		break;
		
		case CJ125_INIT_REG1_STATUS_V17:
		*mode = NORMAL_V17;
		break;
	}
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_read_init1_register (uint16_t *reg)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_INIT_REG1_REQUEST);
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	*reg = statusReg;
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_read_init2_register (uint16_t *reg)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_INIT_REG2_REQUEST);
	
	if (BIT_CHECK(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	*reg = statusReg;
	
	return COMMAND_VALID;
}