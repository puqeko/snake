// code.h
// Two player snake game played with two UCFK's
// Ir codings
//
// By Jozef and Thomas
// Editied 11-10-17

#include "system.h"

 typedef enum CODED_CHARS {
    CODED_NONE = '\0',
    CODED_UP = 'U',
    CODED_DOWN = 'D',
    CODED_LEFT = 'L',
    CODED_RIGHT = 'R',
    CODED_TICK = 'T',
    CODED_READY = 'Z',
    CODED_PASS_CONTROL = 'P',
    CODED_UP_RECEIVED = 'u',
    CODED_DOWN_RECEIVED = 'd',
    CODED_LEFT_RECEIVED = 'l',
    CODED_RIGHT_RECEIVED = 'r',
    CODED_TICK_RECEIVED = 't',
    CODED_READY_RECEIVED = 'z',
    CODED_PASS_CONTROL_RECEIVED = 'p'
} Code;

void code_send(Code code);
Code code_get(void);
void code_update(void);
Code decode_ir(void);
bool code_has_message(void);
void code_init(void);
void code_pass_control(void);