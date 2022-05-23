/*
 * BL49.c
 *
 * Created: 22.05.2022 20:35:45
 * Author : Heinrich
 */ 

#include <avr/io.h>

#include "helpers.h"
#include "adc/adc.h"
#include "spi/spi.h"
#include "cj125/cj125.h"
#include "sensor/sensor.h"
#include "system/system.h"
#include "can/can_lib.h"

void init_gpio_stuff (void);

extern tSensor sensor1;
extern tBoard board;
extern volatile uint16_t counter;

int main(void)
{
	uint8_t signature;
	tcj125_status status;
	
	init_gpio_stuff();
	adc_init();
	spi_init();
	can_init(1);
	
	board.vBatt = adc2voltage_millis(adc_read_battery())*5;
	board.vRef = adc2voltage_millis(adc_read_reference());
	
	signature = cj125_readSignature();
	
	while (status != STATUS_OKAY)
	{
		status = cj125_readStatus();
	}
		
	cj125_set_calibration_mode();
	
	sensor1.Ua_ref = adc2voltage_millis(adc_read_UA());
	sensor1.Ur_ref = adc2voltage_millis(adc_read_UR());
	
	calculate_ip();
	
	int i = 10;
	
    /* Replace with your application code */
    while (1) 
    {
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