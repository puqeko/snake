#include "code.h"
#include "ir_uart.h"
#include "delay.h"


static Code codedOperations[] = {
    CODED_NONE, CODED_UP, CODED_DOWN, CODED_LEFT, CODED_RIGHT, CODED_TICK,
    CODED_READY, CODED_PASS_CONTROL, CODED_UP_RECEIVED, CODED_DOWN_RECEIVED,
    CODED_LEFT_RECEIVED, CODED_RIGHT_RECEIVED, CODED_TICK_RECEIVED,
    CODED_READY_RECEIVED, CODED_PASS_CONTROL_RECEIVED};

#define QU_LENGTH 20

typedef struct queue_s {
    Code messages[QU_LENGTH];
    int head;
    int tail;
    int length;
} Queue;

Queue messages_in = {
    .messages = {0},
    .tail = 0,
    .head = 0
};

Queue messages_out = {
    .messages = {0},
    .tail = 0,
    .head = 0
};

static void queue_push(Queue* qu, Code code)
{
    qu->messages[qu->head++] = code;
    qu->head %= QU_LENGTH;
}

static Code queue_pop(Queue* qu)
{
    Code code = qu->messages[qu->tail++];
    qu->tail %= QU_LENGTH;
    return code;
}


static Code recevied_ir(Code code)
// Converts a to return code
{
    static int numCodedOps = ARRAY_SIZE(codedOperations);

    int pos = 1;
    for (; pos < numCodedOps; pos++) {
        if (codedOperations[pos] == code) {
            break;
        }
    }

    return codedOperations[numCodedOps / 2 + pos];
}


Code decode_ir(void)
// Converts a received char into a Code value or returns
// CODED_NONE if no match is found.
{
    static int numCodedOps = ARRAY_SIZE(codedOperations);

    unsigned char ch = (unsigned char)ir_uart_getc();

    for (int i = 0; i < numCodedOps; i++) {
        if (ch == codedOperations[i]) {
            return codedOperations[i];
        }
    }

    return CODED_NONE;  // No match found
}

static int get_num_messages(Queue* qu)
{
    return (qu->head - qu->tail);
}


void code_send(Code code)
{
    queue_push(&messages_out, code);
}

bool code_has_message(void)
{
    // Check items in list.
    return (get_num_messages(&messages_in) != 0);
}

Code code_get(void)
{
    return queue_pop(&messages_in);
}

void code_update(void)
{
    if (ir_uart_read_ready_p()) {
        Code message = decode_ir();

        queue_push(&messages_in, message);
        // ir_uart_putc(recevied_ir(message));  // Echo back

    } else if (get_num_messages(&messages_out)) {
        Code message = queue_pop(&messages_out);

        // Code receipt = CODED_NONE;
        // Code expectedBack = recevied_ir(message);

        // Resend
        // while (receipt != expectedBack) {
            ir_uart_putc(message);
            // DELAY_US(1000);
            // while (!ir_uart_read_ready_p()) continue;  // TODO: Also wait for time.
            // receipt = decode_ir();
        // }
    }
}