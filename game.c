// game.c
// Two player snake game played with two UCFK's
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17

#include "game.h"
#include "input.h"
#include "snake.h"
#include "board.h"
#include "task.h"
#include "actionbeep.h"

#define SNAKE_UPDATE_RATE (TASK_RATE / 2)  // Snake moves at 2 Hz
#define INPUT_UPDATE_RATE (TASK_RATE / 30)  // Check for input at 30 Hz
#define DISPLAY_UPDATE_RATE (TASK_RATE / DISP_ROWS_UPDATE_FREQ)
#define SOUND_UPDATE_RATE (TASK_RATE / (TONE_FREQUENCY * 2))  // 880 Hz.


static void start_tasks_with_state(State* state)
// Set the task scheduler going as the main game loop.
{
    state->beginTitle(state);

    // The handle a clock from the controlling board.
    // snake_update is a function expecting a state object.
    input_set_controller(snake_update);

    // Array of tasks for task scheduler to run. The tasks share,
    // a reference to the same state object. Cast functions to
    // a type that accepts void* so that we may define handlers
    // with arguments of type State instead.
    task_t tasks[] = {
        {
            .func = (task_func_t)input_update_control,
            .period = SNAKE_UPDATE_RATE,
            .data = state
        }, {
            .func = (task_func_t)input_update,
            .period = INPUT_UPDATE_RATE,
            .data = state
        }, {
            .func = (task_func_t)board_update,
            .period = DISPLAY_UPDATE_RATE,
            .data = state
        }, {
            .func = (task_func_t)sound_update,
            .period = SOUND_UPDATE_RATE,
            .data = state
        }
    };

    // Begin the fun!
    task_schedule (tasks, ARRAY_SIZE(tasks));

    // This function never exits, since task_schedule blocks forever.
}


int main (void)
{
    system_init ();
    sound_init();
    board_init();
    input_init();

    // Keep all game settings and data in one object that is
    // allocated here in the main function.
    State state = {
        .gameMode = GAMEMODE_TITLE,
        .gameBoard = {},  // 7 x 10 array initalised to zero
        .snakeLength = 0, // Boris starts at this length
        .snakeHead = {0, 0},  // Position 0, 0
        .snakeTail = {0, 0},  // Position 0, 0
        .beginEnd = change_state_to_end,
        .beginTitle = reset_state_to_title,
        .beginSnake = change_state_to_snake
    };

    sound_beep(TONE_PUSH_EVENT);  // One beep to signify board starting.
    start_tasks_with_state(&state);

    return 0;
}
