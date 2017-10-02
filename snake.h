// snake.h
// Two player snake game played with two UCFK's
// Internal represenation for the snake.
//
// By Jozef and Thomas
// Editied 01-10-17

enum SNAKE_CELL_TYPE {
    SNAKE_CELL_EMPTY = 0,
    SNAKE_CELL_UP,
    SNAKE_CELL_DOWN,
    SNAKE_CELL_LEFT,
    SNAKE_CELL_RIGHT,
    SNAKE_CELL_FOOD
};

void update_snake(State* state);