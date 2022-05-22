/*
 * pwm.h
 *
 * Created: 22.05.2022 21:21:20
 *  Author: Heinrich
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

void pwm_init (void);
void pwm_setDuty (uint16_t duty);
void pwm_shutdown (void);


#endif /* PWM_H_ */