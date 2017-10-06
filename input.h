// input.h
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

#include "system.h"

#ifndef GAME_INPUT_H
#define GAME_INPUT_H

// Initalise input settings.
void input_init(void);

// Poll for navswitch and button inputs.
void input_update(void* data);

#endif