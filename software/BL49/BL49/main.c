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
	uint8_t counter = 0;
	uint16_t adcValue = 0;
	uint16_t pid = 0;
	tcj125_command_status commStatus;

	adc_init();
	spi_init();
//	dac_init();
	can_network_init(1);

	board_init(&board);	
	sensor_init(&sensor1, 8);
	board_read_inputs(&board);
	
	timer_delay_ms(100);
	
	commStatus = cj125_readSignature(&board.cj125_signature);

	commStatus = cj125_set_calibration_mode();
	timer_delay_ms(500);
	
	
	// Ur value: lower value means hotter sensor...
	// Ur_ref is something round about 1v (1015)
	// Ua_ref is something round about 1.5v (1503)

	// sensor1.Ur_ref_raw = adc_read_UR();
	sensor1.Ur_ref_raw = 96;
	adcValue = adc2voltage_millis(adc_read_UR());
	sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());

	cj125_set_running_mode_v8();
	timer_delay_ms(500);

	sensor_update_status();

	// check activation input before this both steps!!!	
	
	while (counter < 10) 
	{
		sensor_update_status();
		can_send_aem_message(sensor1);
		can_send_debug_message(sensor1.Ur_ref_raw, 0, 0, board.cj125_signature, sensor1.diagRegister);
		
		timer_delay_ms(100);
		counter++;
	}
	
	counter = 0;
	
	sensor1.SensorStatus = EVAP_START_UP;
	
	heater_setVoltage(1500);
	
	while (counter < 25)
	{
		sensor_update_status();
		can_send_aem_message(sensor1);
		can_send_debug_message(sensor1.Ur_ref_raw, 0, 0, board.cj125_signature, sensor1.diagRegister);
		timer_delay_ms(100);
		counter++;
	}
	
	sensor1.SensorStatus = WARMING_UP;
	
	init_1ms_timer();
	
	sei();
	
	while(1)
	{
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_10ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_10ms);
				
			if (sensor1.SensorStatus == RUN && sensor1.SystemVoltageOK == true && sensor1.SensorFaultState == OK)
			{
				sensor_update_ua(&sensor1, adc2voltage_millis(adc_read_UA()));
				dac_update_output(sensor1.Lambda);
			} else {
				dac_setValue(0);
			}
			
			can_send_aem_message(sensor1);			
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_100ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_100ms);
			sensor_update_status();
			board_read_inputs(&board);
			
			if (sensor1.SensorFaultState == OK && sensor1.SensorStatus == RUN && sensor1.SystemVoltageOK)
			{
				adcValue = adc_read_UR();
				pid = calc_pid (sensor1.Ur_ref_raw, adcValue, true);
							
				heater_setVoltage(duty_cycle2voltage(sensor1.SystemVoltage, pid, 256));
				
				sensor_update_ur(&sensor1, adc2voltage_millis(adcValue));
				can_send_debug_message(sensor1.Ur_ref_raw, adcValue, pid, board.cj125_signature, sensor1.diagRegister);
			}
			
			// here we have to check heater und adjust PID!
			// for now we stay at 13v...
			PORTB ^= (1 << PINB5);
		}
		
		if (BIT_CHECK(TIMER_TASKS, BIT_TIMER_250ms))
		{
			BIT_CLEAR(TIMER_TASKS, BIT_TIMER_250ms);
			
			if (sensor1.SensorStatus == WARMING_UP && sensor1.SystemVoltageOK && sensor1.SensorFaultState == OK)
			{
				// we are warming up the sensor in 0.4v/s or 0.1v in 250ms
				sensor1.HeaterVoltage += 100;
				if (sensor1.HeaterVoltage > 13000)
				{
					sensor1.HeaterVoltage = 13000;	
					sensor1.SensorStatus = RUN;
				}
				
				adcValue = adc_read_UR();
				can_send_debug_message(sensor1.Ur_ref_raw, adcValue, 0, board.cj125_signature, sensor1.diagRegister);
				
				heater_setVoltage(sensor1.HeaterVoltage);				
			}
		}
	}
}