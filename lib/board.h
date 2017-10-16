// board.h
// Two player snake game played with two UCFKs
// Display the board to the led matrix.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17

#include "game.h"

#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#define LEDMAT_ROWS_NUM 7

// Configure LED display
void board_init(void);

// Show the snake on the LED matrix
void board_update(State* state);

// Changes game mode to snake. The state struct's gameMode string is not changed
void change_state_to_snake(State* state);

// Changes gamemode to end and changes the string to "Game over!". Also resets strings and LED settings
void change_state_to_end(State* state);

// Reset to the back to the title screen. Also reset the game mode string and LED settings
void reset_state_to_title(State* state);

#endif