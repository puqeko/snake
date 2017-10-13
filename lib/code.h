// code.h
// Two player snake game played with two UCFK's
// Ir codings
//
// By Jozef and Thomas
// Editied 11-10-17

#include "system.h"

#ifndef GAME_CODE_H
#define GAME_CODE_H

// Codes we can send and receive.
typedef enum CODED_CHARS {
    CODED_NONE = '\0',
    CODED_UP = 'U',
    CODED_DOWN = 'D',
    CODED_LEFT = 'L',
    CODED_RIGHT = 'R',
    CODED_TICK = 'T',
    CODED_READY = 'Z',
    CODED_PASS_CONTROL = 'P',
    CODED_EAT = 'E'
} Code;


// Initalise IR registers.
void code_init(void);

// Send a code.
void code_send(Code code);

// Block excecution to send code. Only safe to call this
// once per frame.
void code_send_now(Code code);

// Get next code.
Code code_get(void);

// Tell other board to take control of the snake.
void code_pass_control(void);

// Empty message queue.
void code_clear_messages(void);

// Return true if there are messages wating to be read.
bool code_has_message(void);

// Empty the queue.
void code_clear_messages(void);

// Make sure we are up to date with UART registers.
void code_update(void);

#endif
