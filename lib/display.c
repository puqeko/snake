// display.c
// Two player snake game played with two UCFK's
// Display the board to the led matrix.
//
// By Jozef and Thomas
// Editied 01-10-17

#include "display.h"
#include "pio.h"
#include "ledmat.h"
#include "tinygl.h"


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

void change_state_to_snake(State* state)
// Changes game mode to snake. The state struct's gameMode string is not changed
{
    state->gameMode = GAMEMODE_SNAKE;
}

void change_state_to_end(State* state)
// Changes gamemode to end and changes the string to "Game over!"
{
    char* endString = "Game over!";
    state->gameMode = GAMEMODE_END;
    state->gameModeString = endString;
}

void reset_state_to_title(State* state)
// Reset to the back to the title screen 
{
    state->gameMode = GAMEMODE_TITLE;
    state->gameBoard = {};
    snake_init(state); // Reinitialise the snake information
}
void display_init(void)
// Configure LED display. Also initialises tinygl to run at the display update rate (called tinygl rate), 
// sets the font and the message speed
{
    ledmat_init();
    tinygl_init(TINYGL_UPDATE_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(TINYGL_TEXT_SPEED);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

void display_column(const uint8_t patternArray[GAMEBOARD_ROWS_NUM][GAMEBOARD_COLS_NUM], uint8_t currentColumn)
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
        if (patternArray[currentRow][currentColumn] != SNAKE_CELL_EMPTY) {
            pio_output_low(ledmatRows[currentRow]);
        } else {
            pio_output_high(ledmatRows[currentRow]);
        }
    }

    // Enable new column.
    pio_output_low(ledmatCols[currentColumn]);
    previousColumn = currentColumn;
}



void display_update(State* state)
// Show the snake on the LED matrix.
{
    if (state->gameMode == GAMEMODE_TITLE) {
        char* titleString = "SNAKE - Push when ready";
        tinygl_text(titleString);
        tinygl_update();
    }
    if (state->gameMode == GAMEMODE_END) {
        char* endString = "Game over!";
        tinygl_text(endString);
        tinygl_update();
    }
    if (state->gameMode == GAMEMODE_SNAKE) {
        static int col = 0;  // static to remember value between updates.
        display_column(state->gameBoard, col);

        // Wrap row round so that it iterates through the rows.
        // Therefore, the true refresh rate of the display is
        // the rate this function is called divided by the number
        // of rows in the LED matrix.
        col++;
        if (col == LEDMAT_COLS_NUM) {
            col = 0;
        }
    }
}