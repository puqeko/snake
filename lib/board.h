// board.h
// Two player snake game played with two UCFKs
// Display the board to the led matrix.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17
//
// Directions:
// board_init to configure the display. Use either board_draw to show the snake
// or board_draw_text to show the set text, not both in the same update. 
// board_set_text changes the text.


#include "game.h"


#ifndef GAME_BOARD_H
#define GAME_BOARD_H


// Configure LED display.
void board_init(void);


// Show the snake on the LED matrix.
void board_draw(State* state);


// Show text.
void board_draw_text(void);


// Set text to scroll on screen. Call with NULL to clear the text.
void board_set_text(char* text);


#endif