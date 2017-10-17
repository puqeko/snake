// code.h
// Two player snake game played with two UCFK's
//
// Instructions:
// Messages are transmitted between Fun kits using codes.
// Call code_init() before using other functions.
// Check if there are messages to read with code_has_message() and then
// use code_get() to receive the message as a Code.
// Use code_send(code) to send a message at any time. The code will be added
// to the outgoing queue and sent on the next code_update if possible.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 11-10-17

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

// Get next code.
Code code_get(void);

// Empty message queue.
void code_clear_messages(void);

// Return true if there are messages wating to be read.
bool code_has_message(void);

// Make sure we are up to date with UART registers.
void code_update(void);

#endif
