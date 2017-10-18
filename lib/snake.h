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

// Create food at random position on this board.
void snake_spawn_food(State* state);
void snake_init_spawner(void);  // Call once before spawning.

// Reset the board.
void snake_clear(State* state);

// Set the direction of travel for the next tick.
void snake_set_direction(State* state, SnakeCell newDirection);

#endif