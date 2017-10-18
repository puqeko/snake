// snake.h
// Two player snake game played with two UCFK's
// Update a model for the snake.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17
//
// Directions:
// Used for setting snake behaviour. Call snake_update to progress
// the snake by one pixel. Reset the grid with snake_clear. After reset,
// you should call snake_init to reposition the snake for the start of
// a new game.
//
// Food can be placed randomly on the board with snake_spawn_food. Using
// snake_init_spawner seeds the random generator using a timer for
// a different sequence each game run.
//
// Set a new direction for the snake with snake_set_direction.


#include "game.h"


#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H


// Set initial snake for game start.
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