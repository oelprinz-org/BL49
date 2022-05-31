/*
 * pwm.h
 *
 * Created: 22.05.2022 21:21:20
 *  Author: Heinrich
 */ 


#ifndef HEATER_H_
#define HEATER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

void heater_init (void);
void heater_setDuty (uint16_t duty);
void heater_shutdown (void);
uint16_t heater_pid_control (uint16_t Ur, uint16_t Ur_calibration);


#endif /* HEATER_H_ */