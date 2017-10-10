// display.h
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"

#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#define LEDMAT_ROWS_NUM 7

// Configure LED display
void board_init(void);

// Show the snake on the LED matrix
void board_update(State* state);

#endif