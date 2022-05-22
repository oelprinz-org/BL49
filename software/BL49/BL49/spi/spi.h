/*
 * spi.h
 *
 * Created: 22.05.2022 20:49:21
 *  Author: Heinrich
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

#include "../helpers.h"

#define SPI_SS_SELECT	(PORTD &= ~(1 << PD5))
#define SPI_SS_DESELECT	(PORTD |= (1 << PD5))

void spi_init (void);
uint16_t spi_read_write (uint16_t data);

#endif /* SPI_H_ */