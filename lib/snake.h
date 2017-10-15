// snake.h
// Two player snake game played with two UCFK's
// Internal represenation for the snake.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17

#include "game.h"

#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H

// Set initial gameboard (starting snake)
void snake_init(State* state);

// Update the positions of the snake.
void snake_update(State* state);

#endif