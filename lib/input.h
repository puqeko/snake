// input.h
// Two player snake game played with two UCFK's
// Button controls
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17

#include "game.h"

#ifndef GAME_INPUT_H
#define GAME_INPUT_H

// Initalise input settings.
void input_init(void);

void input_update(void);

// Poll for navswitch and button inputs.
void input_update_internal(State* state);

// Get input from other board.
void input_update_external(State* state);

// True if the navswitch was pressed down.
bool input_did_press_switch(void);

#endif