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
#include "actionbeep.h"

#define SNAKE_UPDATE_RATE (TASK_RATE / 2)
#define INPUT_UPDATE_RATE (TASK_RATE / 30)
#define DISPLAY_UPDATE_RATE (TASK_RATE / DISP_ROWS_UPDATE_FREQ)


// TODO:
// Cannot go back on itself (i.e. opp direction)
// Communicate that gamemode changed to


void sound_update(State* state)
// Update at 880 Hz
{
    static int count = 0;
    static bool shouldWait = false;

    if (shouldWait) {
        count++;

        // Do nothing...

        if (count >= CYCLE_COUNT) {
            count = 0;
            shouldWait = false;
        }
    } else if (state->beepCount > 0) {
        count++;

        // Vibrating speaker.
        pio_output_toggle(PIEZO1_PIO);
        pio_output_toggle(PIEZO2_PIO);

        if (count >= CYCLE_COUNT) {
            count = 0;

            state->beepCount--;
            if (state->beepCount > 0) {
                shouldWait = true;
            }
        }
    }
}

int main (void)
{
    system_init ();
    beeper_init();
    board_init();
    input_init();
    input_set_controller(snake_update);

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
        .beginSnake = change_state_to_snake,
        .beepCount = 1
    };

    // snake_init(&state);
    state.beginTitle(&state);

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
        }, {
            .func = (task_func_t)sound_update,
            .period = TASK_RATE / LOOP_RATE,
            .data = &state
        }
    };

    // Begin the fun!
    task_schedule (tasks, ARRAY_SIZE(tasks));

    return 0;
}
