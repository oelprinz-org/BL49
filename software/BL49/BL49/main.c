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
#include "cj125/cj125.h"
#include "timer/timer.h"
#include "sensor/sensor.h"
#include "system/system.h"

void init_gpio_stuff (void);

extern tSensor sensor1;
extern tBoard board;
extern volatile uint16_t ms_counter;

int main(void)
{
	uint8_t signature;
	tcj125_status status;
	
	init_gpio_stuff();
	adc_init();
	spi_init();
	init_1ms_timer();
	
	sei();
	
	// can_init(1);
	
	/*
	
	There are three different types of CAN modules available:
	-> 2.0A - Considers 29 bit ID as an error
	-> 2.0B Passive - Ignores 29 bit ID messages
	-> 2.0B Active - Handles both 11 and 29 bit ID Messages
	
	
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
	
	board.vBatt = adc2voltage_millis(adc_read_battery())*5;
	board.vRef = adc2voltage_millis(adc_read_reference());
	
	signature = cj125_readSignature();
	
	while (status != STATUS_OKAY)
	{
		status = cj125_readStatus();
	}
		
	cj125_set_calibration_mode();
	
	while (status != STATUS_OKAY)
	{
		status = cj125_readStatus();
	}
	
	sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());
	sensor1.Ur_ref = adc2voltage_millis(adc_read_UR());
	
	calculate_ip();
	
	int i = 10;
	
    /* Replace with your application code */
    while (1) 
    {
		// do stuff...
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_10ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_10ms);
			// do some 10ms stuff...
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
			PORTB ^= (1 << PB5);
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_250ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_250ms);
			// do some 250ms stuff...
			PORTB ^= (1 << PB6);
		}
    }
}


void init_gpio_stuff (void)
{
	// init LED2 (pb5) and LED2 (pb6)
	// led1 is power
	DDRB |= (1 << PB5)|(1 << PB6);
	// switch them on
	PORTB |= (1 << PB5)|(1 << PB6);
	
	// pc7 is output:
	DDRC |= (1 << PC7);
	// bring them high:
	PORTC |= (1 << PC7);
}