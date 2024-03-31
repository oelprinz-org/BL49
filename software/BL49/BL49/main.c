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
	dac_init();
	can_network_init(1);

	board_init(&board);	
	sensor_init(&sensor1, 8);
	board_read_inputs(&board);
	
	timer_delay_ms(100);
	
	sei();

	commStatus = cj125_set_calibration_mode();
	
	
	timer_delay_ms(100);
	
	// commStatus = cj125_read_init1_register(&registerData);
	
	
	// Ur value: lower value means hotter sensor...
	// Ur_ref is something round about 1v (ADC 208 = 1015mV)
	// Ua_ref is something round about 1.5v (ADC 309 =  1503mV)
	
	sensor1.Ur_ref_raw = adc_read_UR();
	sensor1.Ur_ref = adc2voltage_millis(sensor1.Ur_ref_raw);
	sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());

	cj125_set_running_mode_v8();
	timer_delay_ms(100);

	sensor_update_status();
	
	init_1ms_timer();
	
	while(1)
	{
		if (bit_check(TIMER_TASKS, BIT_TIMER_10ms))
		{
			bit_clear(TIMER_TASKS, BIT_TIMER_10ms);
			
			if (sensor1.SensorStatus == RUN && sensor1.SystemVoltageOK == true && sensor1.SensorFaultState == OK)
			{
				sensor_update_ua(&sensor1, adc2voltage_millis(adc_read_UA()));
				dac_update_output(sensor1.Lambda);
			}
			
			can_send_aem_message(sensor1);
		}
		
		if (bit_check(TIMER_TASKS, BIT_TIMER_100ms))
		{
			bit_clear(TIMER_TASKS, BIT_TIMER_100ms);
			sensor_update_status();
			
			// is sensor activated? Transition from RESET (inactive) to start up
			if (isActive() && sensor1.SensorStatus == RESET)
			{
				// set sensor to evap_start_up...
				// from now is heater active and starting up
				// first of all, some time (2s are enough) for evaporation (heater = 1,5v)
				sensor1.SensorStatus = EVAP_START_UP;
				heater_setVoltage(1500);
				counter = 0;
				led1_setState(ON);
			} 
			else if (isActive() && sensor1.SensorStatus == EVAP_START_UP)
			{
				// sensor is in evaporation state, so just check counter 
				counter++;
				
				// if time is over, switch status to next status, WARMING UP
				// set heater to 8.5v
				if (counter == 20)
				{
					sensor1.SensorStatus = WARMING_UP;
					heater_setVoltage(8500);
				}
			}
			
			// transition from non-reset (i.e. warming up, evaporation, run, ...) to dactivated:
			else if (!isActive() && sensor1.SensorStatus != RESET)
			{
				sensor_shutdown();
				dac_setValue(0);
				led1_setState(OFF);
			}
			
			else if (!isActive())
			{
				led2_toggle();

			}
		}
		
		if (bit_check(TIMER_TASKS, BIT_TIMER_250ms))
		{
			bit_clear(TIMER_TASKS, BIT_TIMER_250ms);
			
			can_send_debug_message(sensor1.Ur_ref_raw, adcValue, pid, sensor1.Signature, sensor1.diagRegister);
			
			if (isActive() && sensor1.SensorStatus == WARMING_UP && sensor1.SystemVoltageOK && sensor1.SensorFaultState == OK)
			{
				// increase heater voltage
				sensor1.HeaterVoltage += 100;
				
				// heater voltage is 13v, warming up is completed, next status is RUN
				if (sensor1.HeaterVoltage > 13000)
				{
					sensor1.HeaterVoltage = 13000;
					sensor1.SensorStatus = RUN;
				}
				heater_setVoltage(sensor1.HeaterVoltage);
				led2_toggle();
			}
			
			// we are in a running state, just adjust PID and heater
			else if (isActive() && sensor1.SensorStatus == RUN)
			{
				adcValue = adc_read_UR();
				pid = calc_pid(sensor1.Ur_ref_raw, adcValue, true);
				heater_setVoltage(duty_cycle2voltage(sensor1.SystemVoltage, pid, 256));
				sensor1.Ur = adc2voltage_millis(adcValue);
			}
		}
		
		if (bit_check(TIMER_TASKS, BIT_TIMER_500ms))
		{
			bit_clear(TIMER_TASKS, BIT_TIMER_500ms);
			if (isActive() && sensor1.SensorStatus == RUN)
			{
				led2_toggle();
			}
		}
	}
}