// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

#include "input.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "delay.h"
#include "led.h"

static input_controller_update_func_t controllerUpdateFunc;

//                          up, down, left, right, none
static char codingChars[] = {'U', 'D', 'L', 'R'};
static enum SNAKE_CELL codingCells[] = {  // Corrisponding 
    SNAKE_CELL_UP, SNAKE_CELL_DOWN, SNAKE_CELL_LEFT, SNAKE_CELL_RIGHT};
// TODO: make this nice.


void input_init(void)
// TODO: Configure navswitch etc. 
{
    navswitch_init();
    ir_uart_init();
    led_init();
    led_set (LED1, 0);
}


void input_set_controller(input_controller_update_func_t func) {
    controllerUpdateFunc = func;
}


static enum SNAKE_CELL decode_ir_input(char ch)
// Converts a received char into a cell value or returns
// empty if no match is found.
{
    static int numCodingChars = ARRAY_SIZE(codingChars);
    for (int i = 0; i < numCodingChars; i++) {
        if (ch == codingChars[i]) {
            return codingCells[i];
        }
    }

    return SNAKE_CELL_EMPTY;  // No match found
}


static char encode_ir_output(enum SNAKE_CELL cell)
// Converts a cell value into a char for transmission.
{
    static int numCodingCells = ARRAY_SIZE(codingCells);
    for (int i = 0; i < numCodingCells; i++) {
        if (cell == codingCells[i]) {
            return codingChars[i];
        }
    }

    return '\0';  // Invalid input supplied.
}


void read_navswitch_inputs(State* state)
// Poll the navswitch and update the direction of the head of the snake accordingly.
// Send instructions to other board as well.
{
    navswitch_update();
    
    int8_t xPos = state->snakeHead.row;
    int8_t yPos = state->snakeHead.col;

    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        // north navswitch has been pressed so head updated to up
        state->gameBoard[xPos][yPos] = SNAKE_CELL_UP;
        ir_uart_putc_nocheck(encode_ir_output(SNAKE_CELL_UP));
    }
    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        // south navswitch has been pressed so head updated to down
        state->gameBoard[xPos][yPos] = SNAKE_CELL_DOWN;
        ir_uart_putc_nocheck(encode_ir_output(SNAKE_CELL_DOWN));
    }
    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        // west navswitch has been pressed so head updated to left
        state->gameBoard[xPos][yPos] = SNAKE_CELL_LEFT;
        ir_uart_putc_nocheck(encode_ir_output(SNAKE_CELL_LEFT));
    }
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        // east navswitch has been pressed so head updated to right
        state->gameBoard[xPos][yPos] = SNAKE_CELL_RIGHT;
        ir_uart_putc_nocheck(encode_ir_output(SNAKE_CELL_RIGHT));
    }
}


static bool update_control_status(State* state)
// This board is in control of the snake when the head is on
// the first half of this board.
{
    if (state->snakeHead.col < GAMEBOARD_COLS_NUM / 2) {
        state->isInControl = true;
    } else {
        state->isInControl = false;
    }
}


void receive_external_input(State* state)
// TODO: Keep boards in sync.
{
    if (ir_uart_read_ready_p()) {
        enum SNAKE_CELL cell = decode_ir_input(ir_uart_getc());
        int8_t xPos = state->snakeHead.row;
        int8_t yPos = state->snakeHead.col;

        if (cell != SNAKE_CELL_EMPTY) {
            state->gameBoard[xPos][yPos] = cell;
        }
        // Ignore bad input for now.
    }
}


// 2 Hz
void input_update_control(State* state)
{
    // Ensure control status is correct when we enter and leave update control
    // so that changes stay synced to the 2 Hz cycle.
    update_control_status(state);

    if (state->gameMode == GAMEMODE_SNAKE && state->isInControl) {
        // Clock from this board as we are in control.
        controllerUpdateFunc(state);
    }

    // See above comment.
    update_control_status(state);
}


void send_is_ready() {
    ir_uart_putc('A');
}


// 50 Hz
void input_update(State* state)
// TODO: Poll for navswitch and button inputs.
{
    if (state->gameMode == GAMEMODE_TITLE) {

        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            state->isReady = true;

            if (state->isOtherBoardReady) {
                send_is_ready();
                DELAY_US (3000);  // Wait 3 ms for transmission. See ir_uart.c
                state->gameMode = GAMEMODE_SNAKE;

                // Reset for next time.
                state->isReady = false;
                state->isOtherBoardReady = false;
            } else {
                send_is_ready();
            }
        }

        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == 'A') {
                state->isOtherBoardReady = true;
                led_set (LED1, 1);

                if (state->isReady) {
                    state->gameMode = GAMEMODE_SNAKE;

                    // Reset for next time.
                    state->isReady = false;
                    state->isOtherBoardReady = false;
                }
            }
        }

    } else if (state->gameMode == GAMEMODE_SNAKE) {
        if(state->isInControl) {
            read_navswitch_inputs(state);
        } else {
            // Receive input from other board if not in control.
            receive_external_input(state);
        }
<<<<<<< HEAD
    } else if (state->gameMode == GAMEMODE_TITLE) {
        
    } else { // it's GAMEMODE_END
        // do fancy whizz bang graphics, scrolling "Game over" words etc
        //            ^^^^^^^^^^^^^ absolutely
=======
>>>>>>> bc3ea088c4fcf82e87ef06fc41bc33071f6417cf
    }
}