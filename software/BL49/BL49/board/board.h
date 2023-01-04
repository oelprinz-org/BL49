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
#include <stdbool.h>

typedef enum
{
	LOW
	, HIGH
} tInputState;

typedef struct 
{
	tInputState input1_state;
	tInputState input2_state;
} tBoard;

void board_init (tBoard *board);
void board_read_inputs (tBoard *board);
bool isActive (void);

#endif /* BOARD_H_ */
