/*
 * config.h
 *
 * Created: 23.05.2022 16:38:48
 *  Author: Heinrich
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#ifndef F_CPU
#warning "F_CPU was not defined yet, is now defined with 16MHz"
#define F_CPU 16000000     /* crystal with 16 Mhz */
#endif

#ifndef FOSC
#warning "FOSC was not defined yet, is now defined with F_CPU/1000"
#define FOSC	F_CPU/1000
#endif

#ifndef CAN_BAUDRATE
#warning "CAN_BAUDRATE was not defined yet, is now defined with 500kbit"
#define CAN_BAUDRATE	500
#endif

#endif /* CONFIG_H_ */