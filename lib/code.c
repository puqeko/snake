// code.c
// Two player snake game played with two UCFK's
// Ir codings
//
// By Jozef and Thomas
// Editied 11-10-17
//
// Instructions:
//
// Messages are transimmited between Fun kits using codes.
// Call code_init() before using other functions.
// Check if there are messages to read with code_has_message() and then
// use code_get() to receive the message as a Code.
// Use code_send(code) to send a message at any time. The code will be added
// to the outgoing queue and sent on the next code_update if possible.


#include "code.h"
#include "ir_uart.h"
#include "delay.h"

// Message queue length.
// Small since we don't expect many messages to back up.
#define QU_LENGTH 10


// List of legal codes based off the enum defined in code.h.
static Code codedOperations[] = {
    CODED_NONE, CODED_UP, CODED_DOWN, CODED_LEFT, CODED_RIGHT,
    CODED_TICK, CODED_READY, CODED_PASS_CONTROL};

static bool didSendNow = false;  // For urgent messages.


// FILO structure were we take from the tail and add to the
// head of the queue.
typedef struct queue_s {
    Code messages[QU_LENGTH];
    int head;  // Head element of the queue.
    int tail;  // Tail element of the queue.
} Queue;


// Message queues (incomming and outgoing)
Queue messages_in = {0};
Queue messages_out = {0};


static void queue_push(Queue* queue, Code code)
// Add a code to some message queue.
{
    queue->messages[queue->head++] = code;
    queue->head %= QU_LENGTH;
}


static Code queue_pop(Queue* queue)
// Remove a code from some message queue. Return the code
// that was removed. It is assumed the queue was not empty.
{
    Code code = queue->messages[queue->tail++];
    queue->tail %= QU_LENGTH;
    return code;
}


static Code decode_ir(void)
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
    return (get_num_messages(&messages_in) != 0);
}


void code_send(Code code)
// Add a new code to the outgoing queue.
{
    queue_push(&messages_out, code);
}


void code_send_now(Code code)
// Send instantly, but delay the update of message queues.
{
    ir_uart_putc(code);
    didSendNow = true;
}


Code code_get(void)
// Return the next message in the incomming queue.
{
    return queue_pop(&messages_in);
}


void code_pass_control(void)
// Signal the other board to take control of the snake.
{
    // Wait 5 ms to sync ticks between boards on the
    // transistion.
    DELAY_US(6000);
    code_send(CODED_PASS_CONTROL);
}


void code_clear_messages(void)
// Make sure there are no messages.
{
    // Set to overwrite existing elements.
    messages_in.head = 0;
    messages_in.tail = 0;
}


void code_update(void)
// Send and receive codes based on the contents of the incomming
// and outgoing queue's. There can only be one read or write per frame
// so it is recommened that this function be called frequently to clear
// backlogged messages.
{
    if (didSendNow) {
        didSendNow = false;  // Skip this frame.
    } else {
        if (ir_uart_read_ready_p()) {
            // There is a message to read, so decode it and place in the queue.
            Code message = decode_ir();
            queue_push(&messages_in, message);
        } else if (get_num_messages(&messages_out) != 0) {
            // There are messages to send.
            Code message = queue_pop(&messages_out);
            ir_uart_putc(message);
        }
    }
}
