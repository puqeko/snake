// input.h
// Two player snake game played with two UCFK's
// Main game logic. Define the main tasks for the game and delegate between
// the submodules.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17
//
// Directions:
// Add the transition functions to the state object so they can be accessed
// from any task. Update functions are called as tasks at different
// frequencies. control_input_update_control should be called at 2 Hz. This
// will update the snake and clock to other board if this board is in
// control only. Otherwise, updates are received from the other board and
// snake_update will be called from control_input_update at 2 Hz.


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
void control_input_update(State* state);  // Call at 50 Hz.
void control_board_update(State* state);  // Call at very fast.
void control_sound_update(State* state);  // Call at 880 Hz.


// Update the input state. Checks if this board should remain in control.
void control_input_update_control(State* state);  // Call at 2 Hz.

#endif