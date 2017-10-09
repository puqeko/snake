// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

#include "input.h"
#include "navswitch.h"


void input_init(void)
// TODO: Configure navswitch etc. 
{
    navswitch_init();
}


void read_navswitch_inputs(State* state)
// TODO: Poll the navswitch and update the direction of the head of the snake accordingly.
{
    navswitch_update();
    int8_t xPos = state->snakeHead.row;
    int8_t yPos = state->snakeTail.col;

    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        // north navswitch has been pressed so head updated to up
        state->gameBoard[yPos][xPos] = SNAKE_CELL_UP;
    }
    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        // south navswitch has been pressed so head updated to down
        state->gameBoard[yPos][xPos] = SNAKE_CELL_DOWN;
    }
    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        // west navswitch has been pressed so head updated to left
        state->gameBoard[yPos][xPos] = SNAKE_CELL_LEFT;
    }
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        // east navswitch has been pressed so head updated to right
        state->gameBoard[yPos][xPos] = SNAKE_CELL_RIGHT;
    }
}


void ir_transmit_data(State* state)
// TODO: Keep boards in sync.
{

}


void input_update(State* state)
// TODO: Poll for navswitch and button inputs.
{
    if (state->gameMode == GAMEMODE_SNAKE) {
        // Check if it is valid to move the snake to a new position
        // Change gamemode to end if the next move is invalid

        // game mode snake as normal. What to do here?
        read_navswitch_inputs(state);  // Tada!
        

        // Otherwise, update snake position
    } else if (state->gameMode == GAMEMODE_TITLE) {
        // Do things with tinygl and wait for both players to press the button
    } else { // it's GAMEMODE_END
        // do fancy whizz bang graphics, scrolling "Game over" words etc
        //            ^^^^^^^^^^^^^ absolutely
    }

}