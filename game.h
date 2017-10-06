// game.h
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 02-10-17

#include "system.h"


// Game modes. Can add more later if necessary
enum GAMEMODE {
    GAMEMODE_TITLE = 0,
    GAMEMODE_SNAKE,
    GAMEMODE_END
};


// Used to map the snake's position and the position of food.
enum SNAKE_CELL_TYPE {
    SNAKE_CELL_EMPTY = 0,
    SNAKE_CELL_UP,
    SNAKE_CELL_DOWN,
    SNAKE_CELL_LEFT,
    SNAKE_CELL_RIGHT,
    SNAKE_CELL_FOOD
};


// x,y coordinate system
typedef struct position_s {
    uint8_t x, y;
} Position;


// Game state passed around to all functions.
typedef struct state_s {
    int gameMode;
    uint8_t gameBoard[10][7];
    uint8_t snakeLength;
    Position snakeHead;
    Position snakeTail;
} State;