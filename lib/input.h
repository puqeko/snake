// input.h
// Two player snake game played with two UCFK's
// Button controls. Handle input from other board or navswitch.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17
//
// Directions:
// Call input_update for every frame, then choose between input_update_external
// and input_update_internal depending on the source of input required.


#include "game.h"


#ifndef GAME_INPUT_H
#define GAME_INPUT_H


// Initalise input settings.
void input_init(void);


// Update switch status.
void input_update(void);


// Poll for navswitch and button inputs.
void input_update_internal(State* state);


// Get input from other board.
void input_update_external(State* state);


// True if the navswitch was pressed down.
bool input_did_press_switch(void);


#endif