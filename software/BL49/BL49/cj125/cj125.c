/*
 * cj125.c
 *
 * Created: 22.05.2022 23:46:32
 *  Author: Heinrich
 */ 

#include "cj125.h"
#include "../helpers.h"

uint8_t cj125_readSignature (void)
{	
	uint16_t reg;
	
	reg = spi_read_write(CJ125_IDENT_REG_REQUEST);
	
	if (high(reg) == 0x28 || high(reg) == 0x2e)
	{
		return low(reg);
	}
	
	return 0x00;	
}

tcj125_status cj125_readStatus (void)
{
	uint16_t statusReg = 0;
	uint8_t highByte = 0;
	
	statusReg = spi_read_write(CJ125_DIAG_REG_REQUEST);
	
	// depends of last operation (read or write) and depends of parity bit (0 or 1)
	// the high byte of status register can be 0x2e or 0x28, both values are ok.
	// the most important bit for a command is bit0, here is the status of current command
	// bit0 = 0 --> current command is valid
	// bit0 = 1 --> current command is not valid
	// so just check this bit...
	
	highByte = high(statusReg);
	
	if (BIT_CHECK(highByte, CJ125_DIAG_INSTR_F))
	{
		return CJ125_COMMUNICATION_ERROR;
	}
	
	// if the last command is valid, we check the content of low byte:
	
	switch(low(statusReg))
	{
		case CJ125_DIAG_REG_STATUS_OK:
		return CJ125_STATUS_OKAY;
		break;
		
		case CJ125_DIAG_REG_STATUS_NOPOWER:
		return CJ125_STATUS_E_NOPOWER;
		break;
		
		case CJ125_DIAG_REG_STATUS_NOSENSOR:
		return CJ125_STATUS_E_NOSENSOR;
		break;
		
		default:
		return CJ125_STATUS_ERROR;
		break;
	}
}

void cj125_set_calibration_mode (void)
{
	uint16_t retVal = 0;
	
	retVal = spi_read_write(CJ125_INIT_REG1_MODE_CALIBRATE);
}

// amplification = 8, Lambda 0.65
void cj125_set_running_mode_v8 (void)
{
	uint16_t retVal = 0;
	
	retVal = spi_read_write(CJ125_INIT_REG1_MODE_NORMAL_V8);
}

// amplification = 17, Lambda 0.75
void cj125_set_running_mode_v17 (void)
{
	uint16_t retVal = 0;
	
	retVal = spi_read_write(CJ125_INIT_REG1_MODE_NORMAL_V17);
}

uint16_t cj125_read_init1_register ()
{
	return spi_read_write(CJ125_INIT_REG1_REQUEST);
}

uint16_t cj125_read_init2_register ()
{
	return spi_read_write(CJ125_INIT_REG2_REQUEST);
}