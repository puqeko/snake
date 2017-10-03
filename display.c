// displaysnake.h
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "game.h"
#include "display.h"
#include "pio.h"
#include "ledmat.h"

#define LEDMAT_ROWS_NUM 7


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


void display_column (const uint8_t patternArray[], uint8_t currentColumn)
// Draw patternArray to the LED matrix.
{
    // TODO: Depending on the board this function is called by, it needs
    // to take its half of the 7x10 matrix or rather, have its half of the
    // matrix passed into this function.

    static uint8_t previousColumn = 0;
    uint8_t currentRow;

    // Disable previous column to prevent ghosting while rows modified.
    pio_output_high (ledmatCols[previousColumn]);

    // Activate desired rows based on desired pattern.
    for (currentRow = 0; currentRow < LEDMAT_ROWS_NUM; currentRow++) {

        // The rows are active low.
        if (patternArray[currentRow] != SNAKE_EMPTY) {
            pio_output_low (ledmatRows[currentRow]);
        } else {
            pio_output_high (ledmatRows[currentRow]);
        }
    }

    // Enable new column.
    pio_output_low (ledmatCols[currentColumn]);
    previousColumn = currentColumn;
}