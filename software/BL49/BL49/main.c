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
#include "dac/dac.h"
#include "cj125/cj125.h"
#include "timer/timer.h"
#include "sensor/sensor.h"
#include "board/board.h"
#include "can/can_network.h"

extern tSensor sensor1;
extern tBoard board;
extern volatile uint16_t ms_counter;

int main(void)
{
	uint8_t signature;
	uint16_t dac_value = 0;
	uint16_t tmp_voltage = 1500;
	uint8_t loopCounter = 0;
	uint16_t dty = 0;

	adc_init();
	spi_init();
	can_network_init(1);

	board_init(&board);	
	sensor_init(&sensor1, 8);
	board_read_inputs(&board);
	
	sei();	

	cj125_set_calibration_mode();
	timer_delay_ms(500);
	
	// Ur value: lower value means hotter sensor...
	// Ur_ref is something round about 1v (1015)
	// Ua_ref is something round about 1.5v (1503)
	
	sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());
	sensor1.Ur_ref = adc2voltage_millis(adc_read_UR());
	
	cj125_set_running_mode_v8();
	timer_delay_ms(500);
	
	// check activation input before this both steps!!!
	sensor1.Status = EVAP_START_UP;
	heater_setVoltage(8500);
	
	init_1ms_timer();
	
	while(1)
	{
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_10ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_10ms);
			
			sensor_update_ua (&sensor1, 2490);
						
			can_send_aem_message(sensor1, board.vBatt);
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_100ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_100ms);
			board_read_inputs(&board);
			PORTB ^= (1 << PINB5);
		}
	}
}