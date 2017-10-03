// displaysnake.h
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "system.h"


// Draw patternArray to the LED matrix.
void display_column (uint8_t patternArray[], uint8_t currentColumn);