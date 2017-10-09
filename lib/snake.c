// snake.c
// Two player snake game played with two UCFK's
// Snake behaviour.
//
// Note: The snake is herin refered to as "Boris" the snake.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "snake.h"


Position get_next_position(State* state, Position pos)
{   
    pos.row++;
    if (pos.row > GAME_BOARD_ROWS) {
        pos.row = 0;
    }
    // switch(state->gameBoard[pos.row][pos.col]) {
    //     default:
    //     // ERROR, there should be one of 4 values here.
    //     // Fall back to doing the same as going up.
    //     case SNAKE_CELL_UP:
    //         pos.row--;
    //         if (pos.row < 0) {
    //             pos.row = GAME_BOARD_ROWS - 1;
    //         }
    //         break;
    //     case SNAKE_CELL_DOWN:
    //         pos.row++;
    //         if (pos.row > GAME_BOARD_ROWS - 1) {
    //             pos.row = 0;
    //         }
    //         break;
    //     case SNAKE_CELL_LEFT:
    //         pos.col--;
    //         if (pos.col < 0) {
    //             pos.col = GAME_BOARD_COLUMNS - 1;
    //         }
    //         break;
    //     case SNAKE_CELL_RIGHT:
    //         pos.col++;
    //         if (pos.col > GAME_BOARD_COLUMNS - 1) {
    //             pos.col = 0;
    //         }
    //         break;
    // }

    return pos;
}


static bool will_self_intersect(State* state, Position head)
// Check if next a planned movement in current direction will create a
// self intersection by returing true is that is the case.
{
    // If moving snake head in current direction will create intersect
    // then return true.
    return state->gameBoard[head.row][head.col] != SNAKE_CELL_EMPTY &&
           state->gameBoard[head.row][head.col] != SNAKE_CELL_FOOD;
}


static bool will_eat_food(State* state, Position head)
// Return true if when the snakes head moves in direction then it
// will consume food. Boris will be happy (and slowly becomes obese).
{
    // If moving snake head in current direction will create intersect
    // then return true.
    return state->gameBoard[head.row][head.col] == SNAKE_CELL_FOOD;
}


static void run_boris_run(State* state)
// Move Boris by a single position. Make him grow if he also eats food.
{
    Position oldHead = state->snakeHead;
    Position head = get_next_position(state, oldHead);

    if (will_self_intersect(state, head)) {
        // TODO: Transition to game over state.
    }

    if (will_eat_food(state, head)) {
        state->snakeLength++;
    } else {
        // Move tail pointer.
        Position oldTail = state->snakeTail;
        Position tail = get_next_position(state, oldTail);
        
        state->gameBoard[oldTail.row][oldTail.col] = SNAKE_CELL_EMPTY;
        state->snakeTail = tail;  // Update to new position.
    }

    // Update head position.
    // TODO: SEND NEW POSITION (UART)
    state->gameBoard[head.row][head.col] = state->gameBoard[oldHead.row][oldHead.col];
    state->snakeHead = head;
}


void snake_init(State* state)
{
    // Initalise snake from 2, 0 to 2, 2
    state->gameBoard[0][2] = SNAKE_CELL_UP;
    state->gameBoard[1][2] = SNAKE_CELL_UP;
    state->gameBoard[2][2] = SNAKE_CELL_UP;
}


void snake_update(State* state)
// Update positions of the Boris's LED dot positions. If the head
// position moves to an existing LED point that's on,
// Boris has intersected himself - game over. Poor Boris.
{
    static bool test = false;
    if (state->gameMode == GAMEMODE_SNAKE) {
        // Check if it is valid to move the snake to a new position
        // Change gamemode to end if the next move is invalid

        // Otherwise, update snake position
        if (!test) run_boris_run(state);
        //test = true;
    }
    // else do nothing
}
