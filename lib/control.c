// input.c
// Two player snake game played with two UCFK's
// Main game logic. Define the main tasks for the game and delegate between
// the submodules.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17


#include "input.h"
#include "led.h"
#include "code.h"
#include "actionbeep.h"
#include "snake.h"
#include "board.h"


static void init_as_controller_snake(State* state)
// Initalise boris at one cell. He will grow from a length of 1 up to a length
// of 5 after the game has started.
{
    state->isInControl = true;
    snake_init(state);
}


static void init_as_slave_snake(State* state)
// Initalise a mirrored version of boris so that it maintains an off board
// model of boris on the other board.
{
    state->isInControl = false;
    snake_init(state);
    snake_init_spawner();
    snake_spawn_food(state);
}


static void check_for_sync(State* state)
// In begin mode, sync the boards by having them both signify they are read
// by pressing the middle navswitch button.
{
    // Track the status of the other board and this board during the startup
    // phase of the game.
    static bool isOtherBoardReady = false;
    static bool isThisBoardReady = false;

    if (input_did_press_switch()) {

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
            board_set_text(" READY");
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


void control_transition_to_title_mode(State* state)
// Reset to the back to the title screen. Also reset the game mode string
// and LED settings.
{   
    snake_clear(state);
    led_set (LED1, 0);
    board_set_text(" SNAKE: PUSH START");
    state->gameMode = GAMEMODE_TITLE;
}


void control_transition_to_snake_mode(State* state)
// Changes game mode to snake. The state struct's gameMode string is not changed.
{
    state->gameMode = GAMEMODE_SNAKE;
}


void control_transition_to_end_mode(State* state)
// Changes gamemode to end and changes the string to "Game over!".
// Also resets strings and LED settings.
{
    state->gameMode = GAMEMODE_END;

    // Calculate game score by calculating first and second place characters.
    char outText[] = " SCORE:00! PUSH RESET";
    outText[8] = '0' + (state->snakeTrueLength - state->snakeStartLength) % 10;
    outText[7] = '0' + (state->snakeTrueLength - state->snakeStartLength) / 10;

    board_set_text(outText);
    led_set(LED1, false);
}


void control_board_update(State* state)
{    
    if (state->gameMode == GAMEMODE_TITLE || state->gameMode == GAMEMODE_END) {
        // Update the scrolling text.
        board_draw_text();
    } else if (state->gameMode == GAMEMODE_SNAKE) {
        board_draw(state);
    }
}


void control_sound_update(State* state)
{
    sound_update(state);
}


void control_init(State* state)
// Configure navswtich, led, and code modules.
{
    sound_init();
    board_init();
    input_init();
    code_init();
    led_init();

    led_set(LED1, false);
    sound_beep(TONE_PUSH_EVENT);  // One beep to signify board starting.
    state->beginTitle(state);
}


void control_input_update_control(State* state)
// Check if we should remain in control, make sure to send instructions to the
// other board if so. (2 Hz)
{
    if (state->gameMode == GAMEMODE_SNAKE && state->isInControl) {
        // Clock from this board as we are in control.
        snake_update(state);
        code_send(CODED_TICK);
    }

    // Check if we need to give control to the other board. If so, then
    // update if we are in control and tell the other board. Signal LED
    // to indicate which board is in control.
    bool shouldHaveControl = (state->snakeHead.col < LEDMAT_COLS_NUM);
    if (!shouldHaveControl && state->isInControl) {
        // Give other board control of the snake.
        code_send(CODED_PASS_CONTROL);
        state->isInControl = false;
        led_set (LED1, false);
    }
}


void control_input_update(State* state)
// Poll for navswitch and button inputs. (50 Hz)
{
    input_update();
    code_update();  // Sync with other board.

    if (state->gameMode == GAMEMODE_TITLE) {
        check_for_sync(state);
    } else if (state->gameMode == GAMEMODE_SNAKE) {
        if(state->isInControl) {
            // Get input from this board.
            input_update_internal(state);
        } else {
            // Receive input from other board if not in control.
            input_update_external(state);
        }
    } else if (state->gameMode == GAMEMODE_END) {
        if (input_did_press_switch()) {
            state->beginTitle(state);
            sound_beep(TONE_PUSH_EVENT); // single beep
        }
    }
}