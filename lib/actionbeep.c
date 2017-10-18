// actionbeep.c
// Two player snake game played with two UCFKs.
// Action beep module for initialising and playing a series of configurable beeps.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Editied 15-10-2017


#include "actionbeep.h"
#include "pio.h"


// Connect piezo tweeter to pins 1 and 5 of UCFK4 P1 connector for push-pull operation.
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 1) // changed from 4 so that the piezo speaker would fit
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 5) // NOTE: Don't use 6! Causes the game to fail to transition title>snake


// Desired cycle period for one beep (milliseconds).
#define CYCLE_PERIOD 100
// Number of times to vibrate in sound_update to acheive CYCLE_PERIOD long beeps.
#define CYCLE_COUNT (uint16_t)((CYCLE_PERIOD / 1000.0) * (2 * TONE_FREQUENCY))
#define FOOD_NUM_BEEPS 2  // Two beeps
#define PUSH_NUM_BEEPS 1  // Single beep


static uint16_t numBeepsInput = 0;  // Track the number of beeps still to go.


void sound_init(void)
// Initialise PIO pins to which the piezo speaker is connected.
{
    pio_config_set(PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set(PIEZO2_PIO, PIO_OUTPUT_HIGH);
}


void sound_beep(ToneMode inputSetting)
// Sets the number of remaining beeps depending on the enum toneMode argument.
{
    switch(inputSetting) {
        case TONE_PUSH_EVENT:
            numBeepsInput = PUSH_NUM_BEEPS;
            break;
        case TONE_FOOD_EAT:
            numBeepsInput = FOOD_NUM_BEEPS;
            break;
        default:
            break;  // Do nothing. Invalid code.
    }
}


void sound_update(__unused__ State* state)
// Update at 880 Hz (recommended for 440 Hz sound).
// Currently ouputs with a 50% duty cycle.
{
    static uint16_t count = 0;
    static bool shouldWait = false;

    if (shouldWait) {
        count++;

        // Do nothing...

        if (count >= CYCLE_COUNT) {
            count = 0;
            shouldWait = false;
        }
    } else if (numBeepsInput > 0) {
        count++;

        // Vibrate speaker by swapping polarity.
        pio_output_toggle(PIEZO1_PIO);
        pio_output_toggle(PIEZO2_PIO);

        if (count >= CYCLE_COUNT) {
            count = 0;
            numBeepsInput--;  // We finished one beep.

            if (numBeepsInput > 0) {
                // Wait before the next beep.
                shouldWait = true;
            }
        }
    }
}
