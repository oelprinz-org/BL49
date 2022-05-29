/*
 * board.h
 *
 * Created: 23.05.2022 16:27:08
 *  Author: Heinrich
 */ 


#ifndef BOARD_H_
#define BOARD_H_

#include <avr/io.h>
#include <stdint.h>

typedef enum
{
	BOARD_IDLE
	, BOARD_RUNNING
} tBoardState;

typedef enum
{
	LOW
	, HIGH
} tInputState;

typedef struct 
{
	uint16_t vBatt;
	tInputState input1_state;
	tInputState input2_state;
	tBoardState state;
} tBoard;

extern uint16_t adc_read_battery (void);

void board_init (tBoard *board);
void board_read_inputs (tBoard *board);

#endif /* BOARD_H_ */