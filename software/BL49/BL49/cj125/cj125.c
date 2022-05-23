/*
 * cj125.c
 *
 * Created: 22.05.2022 23:46:32
 *  Author: Heinrich
 */ 

#include "cj125.h"
#include "cj125_registers.h"
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
	
	statusReg = spi_read_write(CJ125_DIAG_REG_REQUEST);
	
	switch(statusReg)
	{
		case CJ125_DIAG_REG_STATUS_OK:
		return STATUS_OKAY;
		break;
			
		case CJ125_DIAG_REG_STATUS_NOPOWER:
		return STATUS_E_NOPOWER;
		break;
			
		case CJ125_DIAG_REG_STATUS_NOSENSOR:
		return STATUS_E_NOSENSOR;
		break;
			
		default:
		return STATUS_ERROR;
		break;
	}
}

void cj125_set_calibration_mode (void)
{
	uint16_t retVal = 0;
	
	retVal = spi_read_write(CJ125_INIT_REG1_MODE_CALIBRATE);
}

void cj125_set_running_mode_v8 (void)
{
	uint16_t retVal = 0;
	
	retVal = spi_read_write(CJ125_INIT_REG1_MODE_NORMAL_V8);
}

void cj125_set_running_mode_v17 (void)
{
	uint16_t retVal = 0;
	
	retVal = spi_read_write(CJ125_INIT_REG1_MODE_NORMAL_V17);
}