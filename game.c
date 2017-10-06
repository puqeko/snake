// game.c
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"
#include "display.h"
//#include "input.h"
//#include "snake.h"
#include "ledmat.h"
#include "led.h"
#include "task.h"

#define SNAKE_UPDATE_RATE (TASK_RATE / 2)
#define INPUT_UPDATE_RATE (TASK_RATE / 50)

// display_update shows only one row at a time so we require it to run
// LEDMAT_ROWS_NUM times at the desired frame rate of 100 Hz.
#define DISPLAY_UPDATE_RATE (TASK_RATE / 500) //(100 * LEDMAT_ROWS_NUM)


void test(void* t)
{
    display_update(t);
}

int main (void)
{
    system_init ();

    // Keep all game settings and data in one object that is
    // allocated here in the main function.
    State state = {
        .gameMode = GAMEMODE_SNAKE,
        .gameBoard = {},  // 7 x 10 array initalised to zero
        .snakeLength = 2, // Boris start's at length 2
        .snakeHead = {2, 2},  // Position 0, 0
        .snakeTail = {2, 0},  // Position 0, 0
    };

    ledmat_init();
    display_init();
    //input_init();
    //snake_init(&state);

    // Initalise snake from 2, 0 to 2, 2
    state.gameBoard[2][2] = SNAKE_CELL_UP;
    state.gameBoard[2][1] = SNAKE_CELL_UP;
    state.gameBoard[2][0] = SNAKE_CELL_UP;

    // Array of tasks for task scheduler to run. The tasks share,
    // a reference to the same state object.
    task_t tasks[] = {
        // {
        //     .func = snake_update,
        //     .period = SNAKE_UPDATE_RATE,
        //     .data = &state
        // }, {
        //     .func = input_update,
        //     .period = INPUT_UPDATE_RATE,
        //     .data = &state
        // },
        {
            .func = test,
            .period = TASK_RATE / 500,
            .data = &state
        }
    };

    led_init();
    led_set(LED1, false);
    ledmat_init();

    task_schedule (tasks, ARRAY_SIZE(tasks));
}