// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By Jozef and Thomas
// Editied 02-10-17

// TODO: Transfer ownership properly.
// If given control but not in control then catchup.

#include "input.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "delay.h"
#include "led.h"

static input_controller_update_func_t controllerUpdateFunc;
static bool previousControlState = false;
static bool waitOneTick = false;
static bool waiting = false;

typedef enum CODED_CHARS Code;
static enum CODED_CHARS {
    CODED_NONE = '\0',
    CODED_UP = 'U',
    CODED_DOWN = 'D',
    CODED_LEFT = 'L',
    CODED_RIGHT = 'R',
    CODED_TICK = 'a',
    CODED_READY = 'z',
    CODED_PASS_CONTROL = 'P',
    CODED_TICK_RECEIVED = 'T',
    CODED_PASS_RECEIVED = 'H'
};
static Code codedOperations[] = {CODED_NONE, CODED_UP, CODED_DOWN, CODED_LEFT, CODED_RIGHT, CODED_TICK, CODED_READY, CODED_PASS_CONTROL, CODED_TICK_RECEIVED, CODED_PASS_RECEIVED};

// ir_uart_getc()
static Code decode_ir(unsigned char ch)
// Converts a received char into a Code value or returns
// CODED_NONE if no match is found.
{
    static int numCodedOps = ARRAY_SIZE(codedOperations);

    for (int i = 0; i < numCodedOps; i++) {
        if (ch == codedOperations[i]) {
            return codedOperations[i];
        }
    }

    return CODED_NONE;  // No match found
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
        ir_uart_putc_nocheck(CODED_UP);
    }
    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        // south navswitch has been pressed so head updated to down
        state->gameBoard[xPos][yPos] = SNAKE_CELL_DOWN;
        ir_uart_putc_nocheck(CODED_DOWN);
    }
    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        // west navswitch has been pressed so head updated to left
        state->gameBoard[xPos][yPos] = SNAKE_CELL_LEFT;
        ir_uart_putc_nocheck(CODED_LEFT);
    }
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        // east navswitch has been pressed so head updated to right
        state->gameBoard[xPos][yPos] = SNAKE_CELL_RIGHT;
        ir_uart_putc_nocheck(CODED_RIGHT);
    }
}


static void update_control_status(State* state)
// This board is in control of the snake when the head is on
// the first half of this board.
{
    bool shouldHaveControl = (state->snakeHead.col < GAMEBOARD_COLS_NUM / 2);
    if (!shouldHaveControl && state->isInControl && !waitOneTick) {
        // while (!ir_uart_write_ready_p()) continue;
        // ir_uart_putc(CODED_PASS_CONTROL);
        // while (!ir_uart_read_ready_p()) continue;
        // ir_uart_getc();  // TODO: retry on fail.
        // state->isInControl = false;
    }
    if (!waitOneTick && waiting) {
        state->isInControl = true;
        waiting = false;
    } else if (!waiting) {
        if (shouldHaveControl && !previousControlState) {
            waitOneTick = true;
            waiting = true;
        } else if (!shouldHaveControl && previousControlState) {
            state->isInControl = false;
        }
    }
     previousControlState = shouldHaveControl;
}


static void receive_external_input(State* state)
// TODO: Keep boards in sync.
{
    if (ir_uart_read_ready_p()) {
        led_set (LED1, 1);
        Code message = decode_ir(ir_uart_getc());
        int row = state->snakeHead.row;
        int col = state->snakeHead.col;
        
        switch(message) {
            case CODED_UP:
                state->gameBoard[row][col] = SNAKE_CELL_DOWN;
                break;
            case CODED_DOWN:
                state->gameBoard[row][col] = SNAKE_CELL_UP;
                break;
            case CODED_LEFT:
                state->gameBoard[row][col] = SNAKE_CELL_RIGHT;
                break;
            case CODED_RIGHT:
                state->gameBoard[row][col] = SNAKE_CELL_LEFT;
                break;
            case CODED_TICK:
                ir_uart_putc(CODED_TICK_RECEIVED);
                //DELAY_US(3000);  // wait for other
                controllerUpdateFunc(state);
                update_control_status(state);
                break;
            case CODED_PASS_CONTROL:
                ir_uart_putc(CODED_PASS_RECEIVED);
                state->isInControl = true;
                state->gameBoard[3][9] = SNAKE_CELL_FOOD;
                state->gameBoard[3][0] = SNAKE_CELL_FOOD;
                
                break;
            case CODED_READY:
            case CODED_NONE:
            default:
                // Ignore bad input for now.
                break;
        }
    } else {
        led_set (LED1, 0);
    }
}


// 2 Hz
void input_update_control(State* state)
{
    // Ensure control status is correct when we enter and leave update control
    // so that changes stay synced to the 2 Hz cycle.
    //update_control_status(state);

    if (state->gameMode == GAMEMODE_SNAKE && state->isInControl && !waitOneTick) {
        // Clock from this board as we are in control.
        ir_uart_putc(CODED_TICK);
        while (!ir_uart_read_ready_p()) continue;
        ir_uart_getc();  // CODED_TICK_RECEIVED. TODO: retry on fail.
        controllerUpdateFunc(state);
    }

    waitOneTick = false;

    // See above comment.
    update_control_status(state);
}


void input_init(void)
// TODO: Configure navswitch etc. 
{
    navswitch_init();
    ir_uart_init();
    led_init();
    led_set(LED1, 0);
}


void input_set_controller(input_controller_update_func_t func) {
    controllerUpdateFunc = func;
}

void init_as_controller_snake(State* state)
{
    state->isInControl = true;
    previousControlState = true;

    // Initalise snake from 2, 0 to 2, 2
    state->gameBoard[0][2] = SNAKE_CELL_UP;
    state->gameBoard[1][2] = SNAKE_CELL_UP;
    state->gameBoard[2][2] = SNAKE_CELL_UP;
    state->gameBoard[3][2] = SNAKE_CELL_UP;
    state->gameBoard[4][2] = SNAKE_CELL_UP;

    Position head = {0, 2};
    Position tail = {4, 2};
    state->snakeHead = head;
    state->snakeTail = tail;

    state->snakeLength = 5;
}

void init_as_slave_snake(State* state)
{
    state->isInControl = false;

    // Initalise snake mirrored
    state->gameBoard[6][7] = SNAKE_CELL_DOWN;
    state->gameBoard[5][7] = SNAKE_CELL_DOWN;
    state->gameBoard[4][7] = SNAKE_CELL_DOWN;
    state->gameBoard[3][7] = SNAKE_CELL_DOWN;
    state->gameBoard[2][7] = SNAKE_CELL_DOWN;

    Position tail = {2, 7};
    Position head = {6, 7};
    state->snakeHead = head;
    state->snakeTail = tail;

    state->snakeLength = 5;
}


// 50 Hz
void input_update(State* state)
// TODO: Poll for navswitch and button inputs.
{
    if (state->gameMode == GAMEMODE_TITLE) {

        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            state->isReady = true;
            ir_uart_putc(CODED_READY);

            if (state->isOtherBoardReady) {
                
                DELAY_US (1000);  // Wait 1 ms for transmission. See ir_uart.c
                if (ir_uart_read_ready_p()) ir_uart_getc();

                state->gameMode = GAMEMODE_SNAKE;

                // Reset for next time.
                state->isReady = false;
                state->isOtherBoardReady = false;
                init_as_slave_snake(state);
                led_set (LED1, 0);
            }
        }

        if (ir_uart_read_ready_p()) {
            if (decode_ir(ir_uart_getc()) == CODED_READY) {
                state->isOtherBoardReady = true;
                led_set (LED1, 1);

                if (state->isReady) {
                    state->gameMode = GAMEMODE_SNAKE;

                    // Reset for next time.
                    state->isReady = false;
                    state->isOtherBoardReady = false;
                    init_as_controller_snake(state);
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
    }
}

