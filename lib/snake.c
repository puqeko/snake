// snake.c
// Two player snake game played with two UCFK's
// Snake behaviour.
//
// Note: The snake is herin refered to as "Boris" the snake.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "snake.h"

static Position get_next_position(State* state, Position currHeadPos) 
// Takes the head position of the snake and determines its next position depending on whether it's an
// up, down, left or right enum. Returns a position struct
{
    enum SNAKE_CELL directionType = state->gameBoard[currHeadPos.row][currHeadPos.col];

    if (directionType == SNAKE_CELL_UP) {
        currHeadPos.row = (currHeadPos.row - 1 + GAMEBOARD_ROWS_NUM) % GAMEBOARD_ROWS_NUM;
    }
    if (directionType == SNAKE_CELL_DOWN) {
        currHeadPos.row = (currHeadPos.row + 1) % GAMEBOARD_ROWS_NUM;
    }
    if (directionType == SNAKE_CELL_LEFT) {
        currHeadPos.col = (currHeadPos.col - 1 + GAMEBOARD_COLS_NUM) % GAMEBOARD_COLS_NUM;
    }
    if (directionType == SNAKE_CELL_RIGHT) {
        currHeadPos.col = (currHeadPos.col + 1) % GAMEBOARD_COLS_NUM;
    }

    return currHeadPos;
}


static bool will_self_intersect(State* state, Position nextHeadPos) 
// Returns true if the next head position is already occupied by an enum other than SNAKE_CELL_EMPTY
{
    return state->gameBoard[nextHeadPos.row][nextHeadPos.col] != SNAKE_CELL_EMPTY &&
           state->gameBoard[nextHeadPos.row][nextHeadPos.col] != SNAKE_CELL_FOOD;
}


static bool will_eat_food(State* state, Position head)
// Return true if when the snakes head moves in direction then it
// will consume food. Boris will be happy (and slowly becomes obese).
{
    // If moving snake head in current direction will create intersect with 
    // then return true.
    return state->gameBoard[head.row][head.col] == SNAKE_CELL_FOOD;
}


static void run_boris_run(State* state)
// Move Boris by a single position. Make him grow if he also eats food.
{
    Position oldHead = state->snakeHead;
    Position head = get_next_position(state, oldHead);

    if (will_self_intersect(state, head)) {
        state->gameMode = GAMEMODE_END;
        return;  // We are done here.
    }

    if (will_eat_food(state, head)) {
        state->snakeLength++;
    } else {
        // Move tail pointer.
        Position oldTail = state->snakeTail;
        Position tail = get_next_position(state, oldTail);
        
        // Remove previous cell.
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

    // state->isInControl = true;
    state->gameMode = GAMEMODE_TITLE;

    // state->isReady = false;
    // state->isOtherBoardReady = false;

    //     state->gameMode = GAMEMODE_TITLE;

    //     // Initalise snake from 2, 0 to 2, 2
    //     state->gameBoard[6][7] = SNAKE_CELL_DOWN;
    //     state->gameBoard[5][7] = SNAKE_CELL_DOWN;
    //     state->gameBoard[4][7] = SNAKE_CELL_DOWN;
    //     state->gameBoard[3][7] = SNAKE_CELL_DOWN;
    //     state->gameBoard[2][7] = SNAKE_CELL_DOWN;
    
    //     Position head = {2, 7};
    //     Position tail = {6, 7};
    //     state->snakeHead = head;
    //     state->snakeTail = tail;
    
    //     state->snakeLength = 5;
    
    //     state->isInControl = false;
}


void snake_update(State* state)
// Update positions of the Boris's LED dot positions. If the head
// position moves to an existing LED point that's on,
// Boris has intersected himself - game over. Poor Boris.
{
    if (state->gameMode == GAMEMODE_SNAKE) {
        // Update snake position.
        run_boris_run(state);
    }
    // else do nothing
}
