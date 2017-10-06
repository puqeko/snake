// game.c
// Two player snake game played with two UCFK's
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"
//#include "input.h"
//#include "snake.h"
#include "ledmat.h"
#include "pio.h"
#include "led.h"
#include "task.h"

#define SNAKE_UPDATE_RATE (TASK_RATE / 2)
#define INPUT_UPDATE_RATE (TASK_RATE / 50)

// display_update shows only one row at a time so we require it to run
// LEDMAT_ROWS_NUM times at the desired frame rate of 100 Hz.
#define DISPLAY_UPDATE_RATE (TASK_RATE / 500) //(100 * LEDMAT_ROWS_NUM)

// Define PIO pins driving LED matrix rows.
static const pio_t ledmatRows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


// Define PIO pins driving LED matrix columns.
static const pio_t ledmatCols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


void display_init(void)
// Configure LED display.
{
    ledmat_init();
}


void display_column(const uint8_t patternArray[], uint8_t currentColumn)
// Draw patternArray to the LED matrix.
{
    // TODO: Depending on the board this function is called by, it needs
    // to take its half of the 7x10 matrix or rather, have its half of the
    // matrix passed into this function.

    static uint8_t previousColumn = 0;
    uint8_t currentRow;

    // Disable previous column to prevent ghosting while rows modified.
    pio_output_high(ledmatCols[previousColumn]);

    // Activate desired rows based on desired pattern.
    for (currentRow = 0; currentRow < LEDMAT_ROWS_NUM; currentRow++) {

        // The rows are active low.
        if (patternArray[currentRow] != SNAKE_CELL_EMPTY) {
            pio_output_low(ledmatRows[currentRow]);
        } else {
            pio_output_high(ledmatRows[currentRow]);
        }
    }

    // Enable new column.
    pio_output_low(ledmatCols[currentColumn]);
    previousColumn = currentColumn;
}

void display_update(void* data)
// Show the snake on the LED matrix.
{
    State* state = (State*) data;
    static int row = 0;

    display_column(state->gameBoard[row], row);
    if (++row == LEDMAT_ROWS_NUM) row = 0;
    // State* state = (State*) data;

    // static int row = 0;  // static to remember value between updates.
    // display_one_column(state->gameBoard[row], row);

    // // Wrap row round so that it iterates through the rows.
    // // Therefore, the true refresh rate of the display is
    // // the rate this function is called divided by the number
    // // of rows in the LED matrix.
    // row++;
    // if (row == LEDMAT_ROWS_NUM) {
    //     row = 0;
    // }
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
            .func = (task_func_t)display_update,
            .period = TASK_RATE / 500,
            .data = &state
        }
    };

    led_init();
    led_set(LED1, false);
    ledmat_init();

    task_schedule (tasks, ARRAY_SIZE(tasks));
}