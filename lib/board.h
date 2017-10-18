// board.h
// Two player snake game played with two UCFKs
// Display the board to the led matrix.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17

#include "game.h"

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

// Configure LED display.
void board_init(void);

// Show the snake on the LED matrix.
void board_draw(State* state);

// Show text.
void board_draw_text(void);

// Set text to scroll on screen.
void board_set_text(char* text);

#endif