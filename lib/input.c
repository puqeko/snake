// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17

#include <stdlib.h>
#include <avr/io.h>
#include "input.h"
#include "navswitch.h"
#include "delay.h"
#include "led.h"
#include "code.h"
#include "actionbeep.h"
#include "tinygl.h"


static input_controller_update_func_t controllerUpdateFunc;


static void set_snake_direction(State* state, SnakeCell newDirection)
{
    int8_t row = state->snakeHead.row;
    int8_t col = state->snakeHead.col;

    SnakeCell currentDirection = state->prevHeadValue;

    if ((newDirection == SNAKE_CELL_LEFT && currentDirection == SNAKE_CELL_RIGHT) ||
        (newDirection == SNAKE_CELL_RIGHT && currentDirection == SNAKE_CELL_LEFT) ||
        (newDirection == SNAKE_CELL_DOWN && currentDirection == SNAKE_CELL_UP) ||
        (newDirection == SNAKE_CELL_UP && currentDirection == SNAKE_CELL_DOWN)) {

        // Ignore boris running back into himself.
        return;
    }

    state->gameBoard[row][col] = newDirection;
}


void read_navswitch_inputs(State* state)
// Poll the navswitch and update the direction of the head of the snake accordingly.
// Send instructions to other board as well.
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        // north navswitch has been pressed so head updated to up
        set_snake_direction(state, SNAKE_CELL_UP);
        code_send(CODED_UP);
    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        // south navswitch has been pressed so head updated to down
        set_snake_direction(state, SNAKE_CELL_DOWN);
        code_send(CODED_DOWN);
    } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        // west navswitch has been pressed so head updated to left
        set_snake_direction(state, SNAKE_CELL_LEFT);
        code_send(CODED_LEFT);
    } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        // east navswitch has been pressed so head updated to right
        set_snake_direction(state, SNAKE_CELL_RIGHT);
        code_send(CODED_RIGHT);
    }
}


static void spawn_food(State* state)
// Randomly place food at an unoccupied cell.
{
    int row = 0;
    int col = 0;

    do {
        row = rand() % 7;
        col = rand() % 5;
        // Keep trying until a legal position is found.
    } while (state->gameBoard[row][col] != SNAKE_CELL_EMPTY);

    state->gameBoard[row][col] = SNAKE_CELL_FOOD;
}


static void receive_external_input(State* state)
// Handle incomming messages when this board is the slave. Set the direction of
// the snake head, update from an external tick, set a new food position, and
// receive control from the other snake.
{
    if (code_has_message()) {
        Code message = code_get();

        // Note: Directions have been reversed because the snake
        // model is mirrorred on the other board relative to this one.
        // Hence, a movement up is actually down, etc.
        switch(message) {
            case CODED_UP:
                set_snake_direction(state, SNAKE_CELL_DOWN);
                break;
            case CODED_DOWN:
                set_snake_direction(state, SNAKE_CELL_UP);
                break;
            case CODED_LEFT:
                set_snake_direction(state, SNAKE_CELL_RIGHT);
                break;
            case CODED_RIGHT:
                set_snake_direction(state, SNAKE_CELL_LEFT);
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
                spawn_food(state);
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
    bool shouldHaveControl = (state->snakeHead.col < GAMEBOARD_COLS_NUM / 2);
    if (!shouldHaveControl && state->isInControl) {
        // Give other board control of the snake.
        code_send(CODED_PASS_CONTROL);
        state->isInControl = false;
        led_set (LED1, false);
    }
}


// 2 Hz
void input_update_control(State* state)
// Check if we should remain in control, make sure to send instructions to the
// other board if so.s
{
    if (state->gameMode == GAMEMODE_SNAKE && state->isInControl) {
        // Clock from this board as we are in control.
        controllerUpdateFunc(state);
        code_send(CODED_TICK);
    }

    input_update_control_status(state);  // look ahead
}


void input_init(void)
// Configure navswtich, led, and code modules.
{
    navswitch_init();
    code_init();
    led_init();
    led_set(LED1, false);
}


void input_set_controller(input_controller_update_func_t func)
// Set the function to call when we are read to updat the snake.
{
    controllerUpdateFunc = func;
}


void init_as_controller_snake(State* state)
// Initalise boris at one cell. He will grow from a length of 1 up to a length
// of 5 after the game has started.
{
    state->isInControl = true;
    state->gameBoard[0][2] = SNAKE_CELL_DOWN;

    Position head = {0, 2};
    Position tail = {0, 2};
    state->snakeHead = head;
    state->snakeTail = tail;

    state->snakeLength = 5;
    state->snakeTrueLength = 1;  // Make it grow to 5.
    state->snakeStartLength = 5;
    state->prevHeadValue = SNAKE_CELL_DOWN;
}

void init_as_slave_snake(State* state)
// Initalise a mirrored version of boris so that it maintains an off board
// model of boris on the other board.
{
    state->isInControl = false;

    // Initalise snake mirrored
    state->gameBoard[6][7] = SNAKE_CELL_UP;

    Position tail = {6, 7};
    Position head = {6, 7};
    state->snakeHead = head;
    state->snakeTail = tail;

    state->snakeLength = 5;
    state->snakeTrueLength = 1;  // Make it grow to 5.
    state->snakeStartLength = 5;
    state->prevHeadValue = SNAKE_CELL_UP;

    srand(TCNT1);  // Seed psudorandom generator from timer value.
    spawn_food(state);
}


void input_check_for_sync(State* state)
// In begin mode, sync the boards by having them both signify they are read
// by pressing the middle navswitch button.
{
    // Track the status of the other board and this board during the startup
    // phase of the game.
    static bool isOtherBoardReady = false;
    static bool isThisBoardReady = false;

    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {

        isThisBoardReady = true;  // This board is ready.

        if (isOtherBoardReady) {
            code_send(CODED_READY);
            code_clear_messages();

            state->beginSnake(state);

            // Reset for next time.
            isOtherBoardReady = isThisBoardReady = false;
            init_as_slave_snake(state);
            led_set (LED1, false);
        } else {
            code_send(CODED_READY);

            // Signal that we are waiting for the other board.
            led_set (LED1, 1);

            tinygl_clear();
            tinygl_text(" READY");
        }
    }

    // Message from other board.
    if (code_has_message() && code_get() == CODED_READY) {
        isOtherBoardReady = true;

        if (isThisBoardReady) {
            code_clear_messages();

            //state->gameMode = GAMEMODE_SNAKE;
            state->beginSnake(state);

            // Reset for next time.
            isOtherBoardReady = isThisBoardReady = false;
            init_as_controller_snake(state);
        }

        sound_beep(TONE_PUSH_EVENT); // single beep
    }
}


// 50 Hz
void input_update(State* state)
// Poll for navswitch and button inputs.
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
            sound_beep(TONE_PUSH_EVENT); // single beep
        }
    }
}
