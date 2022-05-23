/*
 * pid.h
 *
 * Created: 05.12.2021 20:49:44
 *  Author: Heinrich
 */ 


#ifndef PID_H_
#define PID_H_

#include <stdint.h>

//PID regulation variables.
int dState;                                                         /* Last position input. */
int iState;                                                         /* Integrator state. */
const int iMax = 250;                                               /* Maximum allowable integrator state. */
const int iMin = -250;                                              /* Minimum allowable integrator state. */
const float pGain = 120;                                            /* Proportional gain. Default = 120*/
const float iGain = 0.8;                                            /* Integral gain. Default = 0.8*/
const float dGain = 10;                                             /* Derivative gain. Default = 10*/

const uint16_t pwmMin = 0;											/* we have 10 bit fast mode PWM, min value*/
const uint16_t pwmMax = 1023;										/* we have 10 bit fast mode PWM, max value*/

uint16_t pid_control (uint16_t inputValue, uint16_t UR_calibration);

#endif /* PID_H_ */