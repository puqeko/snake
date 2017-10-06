// display.h
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "system.h"

#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#define LEDMAT_ROWS_NUM 7


// Configure LED display
void display_init(void);

// Show the snake on the LED matrix
void display_update(void* data);

#endif