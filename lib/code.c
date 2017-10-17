// code.c
// Two player snake game played with two UCFK's
// Ir codings
//
// Handle communication between boards using coded messages.
// Ensures messages are not sent and receivd at the same time
// by storing them in queues. The update function updates the
// Funkit registers at a constant rate.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Edited 11-10-17

#include "code.h"
#include "ir_uart.h"

// Message queue length.
// Small since we don't expect many messages to back up.
#define QUEUE_BACKLOG_LENGTH 10


// List of legal codes based off the enum defined in code.h.
static Code validCodedOperations[] = {
    CODED_NONE, CODED_UP, CODED_DOWN, CODED_LEFT, CODED_RIGHT,
    CODED_TICK, CODED_READY, CODED_PASS_CONTROL, CODED_EAT
};


// FILO structure were we take from the tail and add to the
// head of the queue.
typedef struct queue_s {
    Code messages[QUEUE_BACKLOG_LENGTH];
    int head;  // Head element of the queue.
    int tail;  // Tail element of the queue.
} Queue;


// Message queues (incomming and outgoing)
Queue messagesIn = {
    .messages = {0},
    .head = 0,
    .tail = 0
};
Queue messagesOut = {
    .messages = {0},
    .head = 0,
    .tail = 0
};


static void queue_push(Queue* queue, Code code)
// Add a code to some message queue.
{
    queue->messages[queue->head++] = code;
    queue->head %= QUEUE_BACKLOG_LENGTH;
}


static Code queue_pop(Queue* queue)
// Remove a code from some message queue. Return the code
// that was removed. It is assumed the queue was not empty.
{
    Code code = queue->messages[queue->tail++];
    queue->tail %= QUEUE_BACKLOG_LENGTH;
    return code;
}


static Code decode_ir(void)
// Converts a received char into a Code value or returns
// CODED_NONE if no match is found.
{
    static int numCodedOps = ARRAY_SIZE(validCodedOperations);
    unsigned char ch = (unsigned char)ir_uart_getc();
    int i;
    
    for (i = 0; i < numCodedOps; i++) {
        if (ch == validCodedOperations[i]) {
            return validCodedOperations[i];
        }
    }

    return CODED_NONE;  // No match found
}


static int get_num_messages(Queue* queue)
// Return the number of messages in some queue.
{
    return (queue->head - queue->tail);
}


void code_init()
// Initalise UART registers.
{
    ir_uart_init();
}


bool code_has_message(void)
// Check if there are any un-read messages.
{
    return (get_num_messages(&messagesIn) != 0);
}


void code_send(Code code)
// Add a new code to the outgoing queue.
{
    queue_push(&messagesOut, code);
}


Code code_get(void)
// Return the next message in the incomming queue.
{
    return queue_pop(&messagesIn);
}


void code_clear_messages(void)
// Make sure there are no messages.
{
    // Set to overwrite existing elements.
    messagesIn.head = 0;
    messagesIn.tail = 0;
}


void code_update(void)
// Send and receive codes based on the contents of the incomming
// and outgoing queue's. There can only be one read or write per frame
// so it is recommened that this function be called frequently to clear
// backlogged messages. Should not update faster than every 4ms since
// sending a character takes about this amount of time.
{
    if (ir_uart_read_ready_p()) {
        // There is a message to read, so decode it and place in the queue.
        Code message = decode_ir();
        queue_push(&messagesIn, message);
    } else if (get_num_messages(&messagesOut) != 0 && ir_uart_write_ready_p()) {
        // There are messages to send.
        Code message = queue_pop(&messagesOut);
        ir_uart_putc(message);
    }
}
