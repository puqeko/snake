// board.c
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 01-10-17

#include "board.h"
#include "pio.h"
#include "ledmat.h"
#include "tinygl.h"
#include "font3x5_1.h"

#define FOOD_DIMNESS_RATE 5  // Higher numbers are dimmer

// Define PIO pins driving LED matrix rows.
static const pio_t ledmatRows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


// Define PIO pins driving LED matrix columns.
static const pio_t ledmatCols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static void apply_column(uint8_t patternArray[GAMEBOARD_ROWS_NUM][GAMEBOARD_COLS_NUM], uint8_t currentColumn)
// Draw patternArray to the LED matrix.
// Depending on the board this function is called by, it needs
// to take its half of the 7x10 matrix or rather, have its half of the
// matrix passed into this function.
{
    static int foodToggle = 0;
    static uint8_t previousColumn = 0;
    uint8_t currentRow;

    // Disable previous column to prevent ghosting while rows modified.
    pio_output_high(ledmatCols[previousColumn]);

    // Activate desired rows based on desired pattern.
    for (currentRow = 0; currentRow < GAMEBOARD_ROWS_NUM; currentRow++) {

        // The rows are active low.
        if (patternArray[currentRow][currentColumn] != SNAKE_CELL_EMPTY) {
            // Treat food differently.
            if (patternArray[currentRow][currentColumn] == SNAKE_CELL_FOOD && foodToggle++) {
                pio_output_high(ledmatRows[currentRow]);
                foodToggle %= FOOD_DIMNESS_RATE;
            } else {
                pio_output_low(ledmatRows[currentRow]);
            }
        } else {
            pio_output_high(ledmatRows[currentRow]);
        }
    }

    // Enable new column.
    pio_output_low(ledmatCols[currentColumn]);
    previousColumn = currentColumn;
}


void board_init(void)
// Configure LED display. Also initialises tinygl to run at the display update rate (called tinygl rate),
// sets the font and the message speed
{
    ledmat_init();  // Configure matrix pins for us.
    tinygl_init(TINYGL_UPDATE_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(TINYGL_TEXT_SPEED);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}


void board_draw(State* state)
// Show the snake on the LED matrix.
{
    static int col = 0;  // Remember rendering column between updates.

    apply_column(state->gameBoard, col);

    // Wrap row round so that it iterates through the rows.
    // Therefore, the true refresh rate of the display is
    // the rate this function is called divided by the number
    // of rows in the LED matrix.
    col++;
    if (col == LEDMAT_COLS_NUM) {
        col = 0;
    }
}


void board_draw_text(void)
// Show text by updating tinygl.
{
    tinygl_update();
}


void board_set_text(char* text)
// Set the scrolling text.
{
    tinygl_clear();
    
    if (text != NULL) {
        tinygl_text(text);
    }
}