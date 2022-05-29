/*
 * cj125_registers.h
 *
 * Created: 22.05.2022 23:51:20
 *  Author: Heinrich
 */ 


#ifndef CJ125_REGISTERS_H_
#define CJ125_REGISTERS_H_

// cj125 registers

#define CJ125_IDENT_REG_REQUEST         0x4800	/* Identify request, gives revision of the chip. */
#define CJ125_INIT_REG1_REQUEST         0x6C00 	/* Requests the first init register. */
#define CJ125_DIAG_REG_REQUEST          0x7800	/* Dignostic request, gives the current status. */
#define CJ125_INIT_REG2_REQUEST         0x7E00  /* Requests the second init register. */

#define CJ125_INIT_REG1_STATUS_CAL      0x289D  /* The response of the init register when in calibration mode. */
#define CJ125_INIT_REG1_STATUS_V8       0x2888  /* The response of the init register when V=8 amplification is in use. */
#define CJ125_INIT_REG1_STATUS_V17      0x2889	/* The response of the init register when V=17 amplification is in use. */

// set status:
#define CJ125_INIT_REG1_MODE_CALIBRATE	0x569D  /* Sets the first init register in calibration mode. */
#define CJ125_INIT_REG1_MODE_NORMAL_V8  0x5688  /* Sets the first init register in operation mode. V=8 amplification. */
#define CJ125_INIT_REG1_MODE_NORMAL_V17 0x5689  /* Sets the first init register in operation mode. V=17 amplification. */

// diag register status:
// #define CJ125_DIAG_REG_STATUS_OK        0x28FF  /* The response of the diagnostic register when everything is ok. */
// #define CJ125_DIAG_REG_STATUS_NOPOWER   0x2855  /* The response of the diagnostic register when power is low. */
// #define CJ125_DIAG_REG_STATUS_NOSENSOR  0x287F  /* The response of the diagnostic register when no sensor is connected. */

// diag register status:
#define CJ125_DIAG_REG_STATUS_OK        0xFF  /* The response of the diagnostic register when everything is ok. */
#define CJ125_DIAG_REG_STATUS_NOPOWER   0x55  /* The response of the diagnostic register when power is low. */
#define CJ125_DIAG_REG_STATUS_NOSENSOR  0x7F  /* The response of the diagnostic register when no sensor is connected. */

#define CJ125_DIAG_INSTR_F				0
#define CJ125_DIAG_PARITY				1
#define CJ125_DIAG_R_W					2

#endif /* CJ125_REGISTERS_H_ */