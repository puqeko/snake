// input.h
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

#include "game.h"

#ifndef GAME_INPUT_H
#define GAME_INPUT_H

typedef void (* input_controller_update_func_t)(State* state);

// Initalise input settings.
void input_init(void);

// Set a function to call at regular intervals weather state
// updated or not. This may be triggered from this boards timer
// or from an external board.
void input_set_controller(input_controller_update_func_t func);

// Poll for navswitch and button inputs.
void input_update(State* state);

// Update the input state regually at lesser intervals if required.
void input_update_control(State* state);

#endif