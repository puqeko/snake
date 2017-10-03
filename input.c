// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

#include "game.h"


void read_navswitch_inputs(void)
// Poll the navswitch and update the direction of the head of the snake accordingly.
{

}


void ir_transmit_data(State* state)
// Keep boards in sync.
{

}


void update_input(State* state)
// Poll for navswitch and button inputs.
{
    if (state->gameMode == GAMEMODE_SNAKE) {
        // Check if it is valid to move the snake to a new position
        // Change gamemode to end if the next move is invalid

        // Otherwise, update snake position
    } else if (state->gameMode == GAMEMODE_TITLE) {
        // Do things
    } // else if ...
}