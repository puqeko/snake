// game.c
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"
#include "display.h"
#include "snake.h"
#include "task.h"

#define SNAKE_UPDATE_RATE TASK_RATE / 2
#define INPUT_UPDATE_RATE TASK_RATE / 50

// display_update shows only one row at a time so we require it to run
// LEDMAT_ROWS_NUM times at the desired frame rate of 100 Hz.
#define DISPLAY_UPDATE_RATE TASK_RATE / (100 * LEDMAT_ROWS_NUM)


int main (void)
{
    system_init ();

    // Keep all game settings and data in one object that is
    // allocated here in the main function.
    State state = {
        .mode = GAMEMODE_TITLE,
        .gameBoard = {},  // 7 x 10 array initalised to zero
        .snakeHead = {},  // Position 0, 0
        .snakeTail = {},  // Position 0, 0
    };

    display_init();
    input_init();
    snake_init(&state);

    // Array of tasks for task scheduler to run. The tasks share,
    // a reference to the same state object.
    task_t tasks[] = {
        {
            .func = snake_update,
            .period = SNAKE_UPDATE_RATE,
            .data = &state
        }, {
            .func = input_update,
            .period = INPUT_UPDATE_RATE,
            .data = &state
        }, {
            .func = display_update,
            .period = DISPLAY_UPDATE_RATE,
            .data = &state
        }
    };

    task_schedule (tasks, ARRAY_SIZE(tasks));
}