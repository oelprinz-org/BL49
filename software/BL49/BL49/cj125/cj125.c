/*
 * cj125.c
 *
 * Created: 22.05.2022 23:46:32
 *  Author: Heinrich
 */ 

#include "cj125.h"
#include "cj125_registers.h"

uint16_t cj125_readSignature (void)
{
	return spi_read_write(CJ125_IDENT_REG_REQUEST);	
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
	}
	
	return STATUS_OKAY;
}