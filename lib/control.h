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
void control_init(State* state);

// Transition between gamemodes.
void control_transition_to_title_mode(State* state);
void control_transition_to_snake_mode(State* state);
void control_transition_to_end_mode(State* state);

// Update logic at different rates in game loop.
void control_input_update(State* state);
void control_board_update(State* state);
void control_sound_update(State* state);

// Update the input state. Checks if this board should remain in control.
void control_input_update_control(State* state);

#endif