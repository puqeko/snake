// game.h
// Two player snake game played with two UCFK's
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17

#include "system.h"
#include "task.h"

#ifndef GAME_H
#define GAME_H

#define GAMEBOARD_ROWS_NUM 7
#define GAMEBOARD_COLS_NUM 10

// board_update shows only one row at a time so we require it to run
// LEDMAT_ROWS_NUM times at the desired frame rate of 100 Hz.
#define TINYGL_TEXT_SPEED 15 // number of letters per 10 seconds
#define DISP_ROWS_UPDATE_FREQ (500 * GAMEBOARD_ROWS_NUM)
#define TINYGL_UPDATE_RATE (DISP_ROWS_UPDATE_FREQ)


// Game modes. Can add more later if necessary
enum GAMEMODE {
    GAMEMODE_TITLE = 0,
    GAMEMODE_SNAKE,
    GAMEMODE_END
};


// Used to map the snake's position and the position of food.
typedef enum SNAKE_CELL {
    SNAKE_CELL_EMPTY = 0,
    SNAKE_CELL_UP,
    SNAKE_CELL_DOWN,
    SNAKE_CELL_LEFT,
    SNAKE_CELL_RIGHT,
    SNAKE_CELL_FOOD
} SnakeCell;


// x,y coordinate system
typedef struct position_s {
    int8_t row, col;
} Position;


typedef struct state_s State;

// Game state passed around to all functions.
struct state_s {
    int gameMode;
    bool isInControl;
    bool isOtherBoardReady;
    bool isReady;

    // Sound
    bool shouldBeep;

    // 7 rows, 10 columns to be consistent with LED matrices.
    uint8_t gameBoard[GAMEBOARD_ROWS_NUM][GAMEBOARD_COLS_NUM];

    // Snake
    uint8_t snakeLength;
    uint8_t snakeTrueLength;
    uint8_t snakeStartLength;
    SnakeCell prevHeadValue;
    Position snakeHead;
    Position snakeTail;
    Position food;

    // Initialise the game mode string as title screen string
    void (* beginSnake)(State* state);
    void (* beginTitle)(State* state);
    void (* beginEnd)(State* state);
};

#endif
