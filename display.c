// display.c
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"
#include "display.h"
#include "pio.h"
#include "ledmat.h"


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


void display_one_column(const uint8_t patternArray[], uint8_t row)
// Draw patternArray to the LED matrix.
{
    static uint8_t col_prev = 0;
    uint8_t row;

    /* Disable previous column to prevent ghosting while rows modified.  */
    pio_output_high (ledmat_cols[col_prev]);

    /* Activate desired rows based on desired pattern.  */
    for (row = 0; row < LEDMAT_ROWS_NUM; row++)
    {
        /* The rows are active low.  */
        if (patternArray[row] != SNAKE_EMPTY)
            pio_output_low (ledmat_rows[row]);
        else
            pio_output_high (ledmat_rows[row]);
    }

    /* Enable new column.  */
    pio_output_low (ledmat_cols[col]);
    col_prev = col;
    // // TODO: Depending on the board this function is called by, it needs
    // // to take its half of the 7x10 matrix or rather, have its half of the
    // // matrix passed into this function.

    // static uint8_t previousColumn = 0;
    // uint8_t currentRow;

    // // Disable previous column to prevent ghosting while rows modified.
    // pio_output_high(ledmatCols[previousColumn]);

    // // Activate desired rows based on desired pattern.
    // for (currentRow = 0; currentRow < LEDMAT_ROWS_NUM; currentRow++) {

    //     // The rows are active low.
    //     if (patternArray[currentRow] != SNAKE_CELL_EMPTY) {
    //         pio_output_low(ledmatRows[currentRow]);
    //     } else {
    //         pio_output_high(ledmatRows[currentRow]);
    //     }
    // }

    // // Enable new column.
    // pio_output_low(ledmatCols[currentColumn]);
    // previousColumn = currentColumn;
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