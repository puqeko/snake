// game.h
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 02-10-17

#include "system.h"

#ifndef GAME_H
#define GAME_H

#define GAMEBOARD_ROWS_NUM 7
#define GAMEBOARD_COLS_NUM 10 

// Game modes. Can add more later if necessary
enum GAMEMODE {
    GAMEMODE_TITLE = 0,
    GAMEMODE_SNAKE,
    GAMEMODE_END
};


// Used to map the snake's position and the position of food.
enum SNAKE_CELL {
    SNAKE_CELL_EMPTY = 0,
    SNAKE_CELL_UP,
    SNAKE_CELL_DOWN,
    SNAKE_CELL_LEFT,
    SNAKE_CELL_RIGHT,
    SNAKE_CELL_FOOD
};


// x,y coordinate system
typedef struct position_s {
    int8_t row, col;
} Position;

typedef struct state_s State;

// Game state passed around to all functions.
struct state_s {
    int gameMode;
    bool isInControl;
    // 7 rows, 10 columns to be consistent with LED matrices.
    uint8_t gameBoard[GAMEBOARD_ROWS_NUM][GAMEBOARD_COLS_NUM]; 
    uint8_t snakeLength;
    Position snakeHead;
    Position snakeTail;
};

#endif
