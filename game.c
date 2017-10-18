// game.c
// Two player snake game played with two UCFK's
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17

#include "game.h"
#include "task.h"
#include "control.h"
#include "actionbeep.h"

#define SNAKE_UPDATE_RATE (TASK_RATE / 2)  // Snake moves at 2 Hz
#define INPUT_UPDATE_RATE (TASK_RATE / 30)  // Check for input at 30 Hz
#define DISPLAY_UPDATE_RATE (TASK_RATE / DISP_ROWS_UPDATE_FREQ)
#define SOUND_UPDATE_RATE (TASK_RATE / (TONE_FREQUENCY * 2))  // 880 Hz.


static void start_tasks_with_state(State* state)
// Set the task scheduler going as the main game loop.
{
    // Array of tasks for task scheduler to run. The tasks share,
    // a reference to the same state object. Cast functions to
    // a type that accepts void* so that we may define handlers
    // with arguments of type State instead.
    task_t tasks[] = {
        {
            .func = (task_func_t)control_input_update_control,
            .period = SNAKE_UPDATE_RATE,
            .data = state
        }, {
            .func = (task_func_t)control_input_update,
            .period = INPUT_UPDATE_RATE,
            .data = state
        }, {
            .func = (task_func_t)control_board_update,
            .period = DISPLAY_UPDATE_RATE,
            .data = state
        }, {
            .func = (task_func_t)control_sound_update,
            .period = SOUND_UPDATE_RATE,
            .data = state
        }
    };

    // Begin the fun!
    control_init(state);
    task_schedule (tasks, ARRAY_SIZE(tasks));
    // This function never exits, since task_schedule blocks forever.
}


int main (void)
{
    system_init ();

    // Keep all game settings and data in one object that is
    // allocated here in the main function.
    State state = {
        .gameMode = GAMEMODE_TITLE,
        .gameBoard = {},  // 7 x 10 array initalised to zero
        .snakeLength = 0, // Boris starts at this length
        .snakeHead = {0, 0},  // Position 0, 0
        .snakeTail = {0, 0},  // Position 0, 0
        .beginEnd = control_transition_to_end_mode,
        .beginTitle = control_transition_to_title_mode,
        .beginSnake = control_transition_to_snake_mode
    };

    start_tasks_with_state(&state);
    return 0;
}
