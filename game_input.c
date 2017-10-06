// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

#include "game.h"
#include "navswitch.h"


void input_init(void)
// TODO: Confiure navswitch etc.
{
    
}


void read_navswitch_inputs(void)
// TODO: Poll the navswitch and update the direction of the head of the snake accordingly.
{

}


void ir_transmit_data(State* state)
// TODO: Keep boards in sync.
{

}


void get_relative_direction(State* state, int direction)
// TODO: Converge a naveswitch direction "direction" into a relative direction that
// depends on the orientation of Boris.
{

}


void input_update(void* data)
// TODO: Poll for navswitch and button inputs.
{
    State* state = (State*) data;

    if (state->gameMode == GAMEMODE_SNAKE) {
        // Check if it is valid to move the snake to a new position
        // Change gamemode to end if the next move is invalid

        // Otherwise, update snake position
    } else if (state->gameMode == GAMEMODE_TITLE) {
        // Do things
    } // else if ...
}