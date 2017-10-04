// game.h
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 02-10-17

#include "system.h"

// Number of elements in an array.
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))


// Game modes. Can add more later if necessary
enum GAMEMODE {
    GAMEMODE_TITLE = 0,
    GAMEMODE_SNAKE,
    GAMEMODE_END
}


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
    int mode;
    uint8_t snakeLength = 2; // Boris start's at length 2
    uint8_t gameBoard[10][7];
    Pos snakeHead;
    Pos snakeTail;
} State;