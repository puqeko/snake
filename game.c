// game.c
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"
#include "input.h"
#include "snake.h"
#include "board.h"
#include "task.h"

#define SNAKE_UPDATE_RATE (TASK_RATE / 2)
#define INPUT_UPDATE_RATE (TASK_RATE / 30)
#define DISPLAY_UPDATE_RATE (TASK_RATE / DISP_ROWS_UPDATE_FREQ)


int main (void)
{
    system_init ();
    board_init();
    input_init();
    input_set_controller(snake_update);

    // Keep all game settings and data in one object that is
    // allocated here in the main function.
    State state = {
        .gameMode = GAMEMODE_TITLE,
        .gameBoard = {},  // 7 x 10 array initalised to zero
        .snakeLength = 0, // Boris starts at this length
        .snakeHead = {},  // Position 0, 0
        .snakeTail = {},  // Position 0, 0
        .gameModeString = "SNAKE - Push to start"
    };

    snake_init(&state);

    // Array of tasks for task scheduler to run. The tasks share,
    // a reference to the same state object.
    task_t tasks[] = {
        {
            .func = (task_func_t)input_update_control,
            .period = SNAKE_UPDATE_RATE,
            .data = &state
        }, {
            .func = (task_func_t)input_update,
            .period = INPUT_UPDATE_RATE,
            .data = &state
        }, {
            .func = (task_func_t)board_update,
            .period = DISPLAY_UPDATE_RATE,
            .data = &state
        }
    };

    // Begin
    task_schedule (tasks, ARRAY_SIZE(tasks));
}
