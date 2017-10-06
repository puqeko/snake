// snake.c
// Two player snake game played with two UCFK's
// Snake behaviour.
//
// Note: The snake is herin refered to as "Boris" the snake.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"  // has stdbool


// Set inital gameboard here.
void snake_init(State* state)
{
    // Initalise snake from 2, 0 to 2, 2
    state->gameBoard[2][2] = SNAKE_CELL_UP;
    state->gameBoard[2][1] = SNAKE_CELL_UP;
    state->gameBoard[2][0] = SNAKE_CELL_UP;
}


bool will_self_intersect(State* state)
// TODO: Check if next a planned movement in current direction will create a
// self intersection by returing true is that is the case.
{
    // If moving snake head in current direction will create intersect
    // then return true.
}


void run_boris_run(State* state, bool doesEatFood)
// TODO: Move Boris by a single position. Make him grow if he also eats food.
{

}


bool will_eat_food(State* state)
// TODO: Return true if when the snakes head moves in direction then it
// will consume food. Boris will be happy.
{
    // If moving snake head in current direction will create intersect
    // then return true.
}


void spawn_food_bit(State* state)
// TODO: Set food bit to a new location that is NOT currently occupied by Boris.
{

}


void snake_update(void* data)
// Update positions of the Boris's LED dot positions. If the head
// position moves to an existing LED point that's on,
// Boris has intersected himself - game over. Poor Boris.
{
    State* state = (State*) data;

    if (state->gameMode == GAMEMODE_SNAKE) {
        // Check if it is valid to move the snake to a new position
        // Change gamemode to end if the next move is invalid

        // Otherwise, update snake position
    }
    // else do nothing
}