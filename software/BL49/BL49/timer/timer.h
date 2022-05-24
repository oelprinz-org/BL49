/*
 * timer.h
 *
 * Created: 23.05.2022 16:45:30
 *  Author: Heinrich
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include "../helpers.h"

#define BIT_TIMER_10ms			0
#define BIT_TIMER_20ms			1
#define BIT_TIMER_50ms			2
#define BIT_TIMER_100ms			3
#define BIT_TIMER_250ms			4

volatile uint8_t TIMER_TASKS;

void init_1ms_timer (void);



#endif /* TIMER_H_ */