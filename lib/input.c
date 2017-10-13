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
#include "delay.h"
#include "led.h"
#include "code.h"
#include <stdlib.h>
#include <avr/io.h>

static input_controller_update_func_t controllerUpdateFunc;

void read_navswitch_inputs(State* state)
// Poll the navswitch and update the direction of the head of the snake accordingly.
// Send instructions to other board as well.
{
    int8_t row = state->snakeHead.row;
    int8_t col = state->snakeHead.col;

    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        // north navswitch has been pressed so head updated to up
        state->gameBoard[row][col] = SNAKE_CELL_UP;
        code_send(CODED_UP);
    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        // south navswitch has been pressed so head updated to down
        state->gameBoard[row][col] = SNAKE_CELL_DOWN;
        code_send(CODED_DOWN);
    } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        // west navswitch has been pressed so head updated to left
        state->gameBoard[row][col] = SNAKE_CELL_LEFT;
        code_send(CODED_LEFT);
    } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        // east navswitch has been pressed so head updated to right
        state->gameBoard[row][col] = SNAKE_CELL_RIGHT;
        code_send(CODED_RIGHT);
    }
}


static void receive_external_input(State* state)
// TODO: Keep boards in sync.
{
    if (code_has_message()) {
        Code message = code_get();
        int row = state->snakeHead.row;
        int col = state->snakeHead.col;
        
        // Note: Directions have been reversed because the snake
        // model is mirrorred on the other board relative to this one.
        // Hence, a movement up is actually down, etc.
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
                controllerUpdateFunc(state);
                break;
            case CODED_PASS_CONTROL:
                state->isInControl = true;
                led_set (LED1, true);
                break;
            case CODED_EAT:
                state->snakeLength++;
                while (true) {
                    int row = rand() % 7;
                    int col = rand() % 5;
        
                    if (state->gameBoard[row][col] == SNAKE_CELL_EMPTY) {
                        state->gameBoard[row][col] = SNAKE_CELL_FOOD;
                        state->food.col = col;
                        state->food.row = row;
                        break;
                    }
                }
                break;
            default:
                // Ignore bad input as noise.
                break;
        }
    }
}


void input_update_control_status(State* state)
// Check if we need to give control to the other board. If so, then
// update if we are in control and tell the other board. Signal LED
// to indicate which board is in control.
{
    // For other side control.
    // bool shouldHaveControl = (state->snakeHead.col >= GAMEBOARD_COLS_NUM / 2);
    bool shouldHaveControl = (state->snakeHead.col < GAMEBOARD_COLS_NUM / 2);
    if (!shouldHaveControl && state->isInControl) {
        code_pass_control();
        state->isInControl = false;
        led_set (LED1, false);
    }
}


// 2 Hz
void input_update_control(State* state)
{   
    if (state->gameMode == GAMEMODE_SNAKE && state->isInControl) {
        // Clock from this board as we are in control.
        controllerUpdateFunc(state);
        code_send(CODED_TICK);
    }

    input_update_control_status(state);  // look ahead
}


void input_init(void)
// TODO: Configure navswitch etc. 
{
    navswitch_init();
    code_init();
    led_init();
    led_set(LED1, false);
}


void input_set_controller(input_controller_update_func_t func) {
    controllerUpdateFunc = func;
}


void init_as_controller_snake(State* state)
{
    state->isInControl = true;
    //previousControlState = true;

    // Initalise snake from 2, 0 to 2, 2
    state->gameBoard[0][2] = SNAKE_CELL_DOWN;
    // state->gameBoard[1][2] = SNAKE_CELL_UP;
    // state->gameBoard[2][2] = SNAKE_CELL_UP;
    // state->gameBoard[3][2] = SNAKE_CELL_UP;
    // state->gameBoard[4][2] = SNAKE_CELL_UP;

    Position head = {0, 2};
    Position tail = {0, 2};
    state->snakeHead = head;
    state->snakeTail = tail;

    state->snakeLength = 5;
    state->snakeTrueLength = 1;  // Make it grow to 5.
    state->snakeStartLength = 5;
}

void init_as_slave_snake(State* state)
{
    state->isInControl = false;

    // Initalise snake mirrored
    state->gameBoard[6][7] = SNAKE_CELL_UP;
    // state->gameBoard[5][7] = SNAKE_CELL_DOWN;
    // state->gameBoard[4][7] = SNAKE_CELL_DOWN;
    // state->gameBoard[3][7] = SNAKE_CELL_DOWN;
    // state->gameBoard[2][7] = SNAKE_CELL_DOWN;

    Position tail = {6, 7};
    Position head = {6, 7};
    state->snakeHead = head;
    state->snakeTail = tail;

    state->snakeLength = 5;
    state->snakeTrueLength = 1;  // Make it grow to 5.
    state->snakeStartLength = 5;

    srand(TCNT1);  // initalise from timer value.
    while (true) {
        int row = rand() % 7;
        int col = rand() % 5;

        if (state->gameBoard[row][col] == SNAKE_CELL_EMPTY) {
            state->gameBoard[row][col] = SNAKE_CELL_FOOD;
            state->food.col = col;
            state->food.row = row;
            break;
        }
    }
}


void input_check_for_sync(State* state)
{
    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {

        // state->gameMode = GAMEMODE_SNAKE;
        // init_as_controller_snake(state);
        
        state->isReady = true;  // This board is ready.

        if (state->isOtherBoardReady) {
            // Wait 1 ms for transmission. See ir_uart.c
            // We need to send instantly for the wait to work.
            code_send_now(CODED_READY);
            DELAY_US (1000);
            code_clear_messages();

            //state->gameMode = GAMEMODE_SNAKE;
            state->beginSnake(state);

            // Reset for next time.
            state->isOtherBoardReady = state->isReady = false;
            init_as_slave_snake(state);
            led_set (LED1, 0);
        } else {
            code_send_now(CODED_READY);
            led_set (LED1, 1);  // Signal that we are waiting for the other board.
        }
    }

    // Message from other board.
    if (code_has_message() && code_get() == CODED_READY) {
        state->isOtherBoardReady = true;

        if (state->isReady) {
            code_clear_messages();

            //state->gameMode = GAMEMODE_SNAKE;
            state->beginSnake(state);

            // Reset for next time.
            state->isOtherBoardReady = state->isReady = false;
            init_as_controller_snake(state);
        }
    }
}


// 50 Hz
void input_update(State* state)
// TODO: Poll for navswitch and button inputs.
{
    navswitch_update();
    code_update();  // Sync with other board.

    if (state->gameMode == GAMEMODE_TITLE) {
        input_check_for_sync(state);
    } else if (state->gameMode == GAMEMODE_SNAKE) {
        if(state->isInControl) {
            // Get input from this board.
            read_navswitch_inputs(state);
        } else {
            // Receive input from other board if not in control.
            receive_external_input(state);
        }
    } else if (state->gameMode == GAMEMODE_END) {
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            state->beginTitle(state);
        }
    }
}
