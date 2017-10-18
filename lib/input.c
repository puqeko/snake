// input.c
// Two player snake game played with two UCFK's
// Button controls
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 02-10-17

#include "navswitch.h"
#include "snake.h"
#include "code.h"
#include "led.h"


void input_init(void)
{
    navswitch_init();
}


void input_update(void)
{
    navswitch_update();
}


void input_update_internal(State* state)
// Poll the navswitch and update the direction of the head of the snake accordingly.
// Send instructions to other board as well.
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        // north navswitch has been pressed so head updated to up
        snake_set_direction(state, SNAKE_CELL_UP);
        code_send(CODED_UP);
    }
    
    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        // south navswitch has been pressed so head updated to down
        snake_set_direction(state, SNAKE_CELL_DOWN);
        code_send(CODED_DOWN);
    }
    
    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        // west navswitch has been pressed so head updated to left
        snake_set_direction(state, SNAKE_CELL_LEFT);
        code_send(CODED_LEFT);
    }
    
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        // east navswitch has been pressed so head updated to right
        snake_set_direction(state, SNAKE_CELL_RIGHT);
        code_send(CODED_RIGHT);
    }
}


void input_update_external(State* state)
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
                snake_set_direction(state, SNAKE_CELL_DOWN);
                break;
            case CODED_DOWN:
                snake_set_direction(state, SNAKE_CELL_UP);
                break;
            case CODED_LEFT:
                snake_set_direction(state, SNAKE_CELL_RIGHT);
                break;
            case CODED_RIGHT:
                snake_set_direction(state, SNAKE_CELL_LEFT);
                break;
            case CODED_TICK:
                snake_update(state);
                break;
            case CODED_PASS_CONTROL:
                state->isInControl = true;
                led_set(LED1, true);
                break;
            case CODED_EAT:
                state->snakeLength++;
                snake_spawn_food(state);
                break;
            default:
                // Ignore bad input as noise.
                break;
        }
    }
}


bool input_did_press_switch(void)
// Alias for navswitch push down.
{
    return navswitch_push_event_p(NAVSWITCH_PUSH);
}