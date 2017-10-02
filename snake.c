// snake.c
// Two player snake game played with two UCFK's
// Snake behaviour.
//
// Note: The snake is herin refered to as "Boris" the snake.
//
// By Jozef and Thomas
// Editied 01-10-17

#include <stdbool.h>


// Check if next a planned movement in current direction will create a
// self intersection by returing true is that is the case.
bool will_self_intersect(State* state)
{
    // If moving snake head in current direction will create intersect
    // then return true.
}


// Return true if when the snakes head moves in direction then it
// will consume food. Boris will be happy.
bool will_eat_food(State* state)
{
    // If moving snake head in current direction will create intersect
    // then return true.
}


// Move Boris by a single position. Make him grow if he also eats food.
void run_boris_run(State* state, bool doesEatFood)
{

}


// Update positions of the Boris's LED dot positions. If the head
// position moves to an existing LED point that's on,
// Boris has intersected himself - game over. Poor Boris.
void update_snake (State* state)
{
    if (state->gameMode == GAMEMODE_SNAKE) {
        // Check if it is valid to move the snake to a new position
        // Change gamemode to end if the next move is invalid

        // Otherwise, update snake position
    }
}