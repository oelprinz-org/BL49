/*
 * BL49.c
 *
 * Created: 22.05.2022 20:35:45
 * Author : Heinrich
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "helpers.h"
#include "adc/adc.h"
#include "spi/spi.h"
#include "can/can_lib.h"
#include "dac/dac.h"
#include "pwm/pwm.h"
#include "cj125/cj125.h"
#include "timer/timer.h"
#include "sensor/sensor.h"
#include "board/board.h"

extern tSensor sensor1;
extern tBoard board;
extern volatile uint16_t ms_counter;

int main(void)
{
	uint8_t signature;
	uint16_t dac_value = 0;
	
	uint8_t loopCounter = 0;

	adc_init();
	spi_init();
	pwm_init();
	init_1ms_timer();
	// can_init(1);

	
	board_init(&board);	
	sensor_init(&sensor1, 61.9, 8);
	board_read_inputs(&board);
	

	sei();

	/*
	There are three different types of CAN modules available:
	-> 2.0A - Considers 29 bit ID as an error
	-> 2.0B Passive - Ignores 29 bit ID messages
	-> 2.0B Active - Handles both 11 and 29 bit ID Messages
	*/
	
	/*	
	st_cmd_t message;
	
	message.id.ext = 0x180;
	// message.ctrl.ide = 0;			// standard CAN frame type (2.0A)
	
	message.ctrl.ide = 1;				// we are using extended ID, check can_lib.c:118
	message.ctrl.rtr = 0;				// this message object is not requesting a remote node to transmit data back
	message.dlc = 1;
	message.cmd = CMD_TX_DATA;
	// message.pt_data = 0x01;
	
	while(can_cmd(&message) != CAN_CMD_ACCEPTED);					// wait for MOb to configure
	while(can_get_status(&message) == CAN_STATUS_NOT_COMPLETED);	// wait for a transmit request to come in, and send a response
	*/
	
	uint16_t dty = 0;
	
	
	
	// pwm_setDuty (dty);

	signature = cj125_readSignature();
	
	// board_read_inputs(&board_read_inputs);

	sensor1.Ur_ref = adc2voltage_millis(adc_read_UR());
	sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());
	sensor1.Ua = 1512;
	
	dac_value = get_dac_value(sensor1.Lambda);
	
    /* Replace with your application code */
    while (1) 
    {
		/*
			PORTC |= (1 << PINC7);
			sensor1.Ua = adc2voltage_millis(adc_read_UA());
			sensor1.Ur = adc2voltage_millis(adc_read_UR());
			calculate_ip(&sensor1);
			calculate_lambda(&sensor1);
			PORTC &= ~(1 << PINC7);
		*/
		// do stuff...
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_10ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_10ms);
			/*
			if (sensor1.State == SENSOR_RUNNING)
			{
				if(cj125_readStatus() == CJ125_STATUS_OKAY)
				{
					calculate_ip(&sensor1);
					calculate_lambda(&sensor1);
					
				}
				else
				{
					
				}
			}
			*/		
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_20ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_20ms);
			// do some 20ms stuff...
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_50ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_50ms);
			// do some 50ms stuff...
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_100ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_100ms);
			// do some 100ms stuff...
			PORTB ^= (1 << PINB5);
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_250ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_250ms);
			// do some 250ms stuff...
			board_read_inputs(&board);
			
			if (board.input1_state == HIGH)
			{
				// means: engine is running
				switch (sensor1.State)
				{
					case SENSOR_OFF:
						// means engine is started right now so we need to init the sensor and stuff...
						cj125_set_calibration_mode();
					
						if (cj125_readStatus() == CJ125_STATUS_OKAY)
						{
							// everything is fine, we are in calibration mode...
							sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());
							sensor1.Ur_ref = adc2voltage_millis(adc_read_UR());
						
							// stuff stored, go for condensation...1.5v for 5 seconds
							// 5 s = 5000 ms = 20
							loopCounter = 19;
							sensor1.State = SENSOR_CONDENSATION;
						}
					break;
					
					case SENSOR_CONDENSATION:
						dty = target_voltage_duty_cycle(1500, board.vBatt);
						pwm_setDuty(dty);
						loopCounter--;
						
						if (loopCounter == 0)
						{
							// condensation ended, next step heating up the sensor
							loopCounter = 
							sensor1.State = SENSOR_HEATING_UP;
						}
						
					case SENSOR_HEATING_UP:
					
					break;
				}
				
				
				
			}

			PORTB ^= (1 << PINB6);
		}

    }
}