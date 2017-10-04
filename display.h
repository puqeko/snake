// display.h
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "system.h"

#define LEDMAT_ROWS_NUM 7


// Configure LED display
void display_init();

// Draw patternArray to the LED matrix.
void display_column (uint8_t patternArray[], uint8_t currentColumn);

// Show the snake on the LED matrix
void display_update(State* state);