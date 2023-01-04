/*
 * cj125.c
 *
 * Created: 22.05.2022 23:46:32
 *  Author: Heinrich
 */ 

#include "cj125.h"
#include "../helpers.h"

tcj125_command_status cj125_sreset (void)
{
	uint16_t reg = 0;
	reg = spi_read_write(CJ125_WR_SRESET);
	
	reg = spi_read_write(CJ125_WR_SRESET_END);
	
	if (high(reg) == 0x28 || high(reg) == 0x2e)
	{
		return COMMAND_VALID;
	}
	
	return COMMAND_NOT_VALID;	
}

tcj125_command_status cj125_readSignature (uint8_t *sig)
{	
	uint16_t reg;
	
	reg = spi_read_write(CJ125_RD_IDENT);
	
	if (high(reg) == 0x28 || high(reg) == 0x2e)
	{
		*sig = (low(reg));
		return COMMAND_VALID;
	}
	
	return COMMAND_NOT_VALID;
}

tcj125_command_status cj125_readStatus (uint8_t *diagRegister)
{
	uint16_t statusReg = 0;
	
	statusReg = spi_read_write(CJ125_RD_DIAG);
	
	// depends of last operation (read or write) and depends of parity bit (0 or 1)
	// the high byte of status register can be 0x2e or 0x28, both values are ok.
	// the most important bit for a command is bit0, here is the status of current command
	// bit0 = 0 --> current command is valid
	// bit0 = 1 --> current command is not valid
	// so just check this bit...
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	// if the last command is valid, we check the content of low byte:
	
	*diagRegister = low (statusReg);
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_set_calibration_mode (void)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_WR_MODE_CAL);
	
	statusReg = spi_read_write(CJ125_RD_INIT1);
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	return COMMAND_VALID;
}

// amplification = 8, Lambda 0.65
tcj125_command_status cj125_set_running_mode_v8 (void)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_WR_MODE_V8);
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	return COMMAND_VALID;
}

// amplification = 17, Lambda 0.75
tcj125_command_status cj125_set_running_mode_v17 (void)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_WR_MODE_V17);
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_get_mode (tcj125_mode *mode)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_RD_INIT1);
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	switch (statusReg)
	{
		case CJ125_RD_MODE_CAL:
		*mode = CALIBRATION;
		break;
		
		case CJ125_RD_MODE_V8:
		*mode = NORMAL_V8;
		break;
		
		case CJ125_RD_MODE_V17:
		*mode = NORMAL_V17;
		break;
	}
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_read_init1_register (uint16_t *reg)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_RD_INIT1);
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	*reg = statusReg;
	
	return COMMAND_VALID;
}

tcj125_command_status cj125_read_init2_register (uint16_t *reg)
{
	uint16_t statusReg = 0;

	statusReg = spi_read_write(CJ125_RD_INIT2);
	
	if (bit_check(high(statusReg), CJ125_DIAG_INSTR_F))
	{
		return COMMAND_NOT_VALID;
	}
	
	*reg = statusReg;
	
	return COMMAND_VALID;
}