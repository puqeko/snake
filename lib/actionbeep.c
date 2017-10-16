// actionbeep.c
// Action beep module for initialising and playing a series of configurable beeps
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Created 15/10/2017

#include "actionbeep.h"
#include "pio.h"

//Connect piezo tweeter to pins 5 and 8 of UCFK4 P1 connector for push-pull operation.
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 1) // changed from 4 so that the piezo speaker would fit
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 5) // DON'T USE 6! Causes the game to fail to transition title>snake

#define CYCLE_PERIOD 100 // desired cycle period in milliseconds
#define CYCLE_COUNT (uint16_t)((CYCLE_PERIOD / 1000.0) * (2 * TONE_FREQUENCY)) // number of times pacer_wait() is called
#define FOOD_NUM_BEEPS 3
#define PUSH_NUM_BEEPS 1

static uint16_t numBeepsInput = 1;

void sound_init(void)
// Initialise PIO pins to which the piezo speaker is connected
{
    pio_config_set(PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set(PIEZO2_PIO, PIO_OUTPUT_HIGH);
}


void sound_beep(ToneMode inputSetting)
// Calls run_beep_cycle with the required number of beep cycles depending on the enum toneMode argument
{
    switch(inputSetting) {
        case TONE_PUSH_EVENT:
            //sound_update(PUSH_NUM_BEEPS);
            numBeepsInput = PUSH_NUM_BEEPS;
            break;
        case TONE_FOOD_EAT:
            //sound_update(FOOD_NUM_BEEPS);
            numBeepsInput = FOOD_NUM_BEEPS;
            break;
        case TONE_END_SOUND:
            sound_end_tone();
            break;
    }
}


void sound_end_tone(void)
{
    // do some fancy pitch / volume changes if possible or time permitting
}


void sound_update(__unused__ State* state)
// Update at 880 Hz
{
    static uint16_t count = 0;
    static bool shouldWait = false;
    // static uint16_t beepCount = numBeepsInput;

    if (shouldWait) {
        count++;
        // Do nothing...

        if (count >= CYCLE_COUNT) {
            count = 0;
            shouldWait = false;
        }
    } else if (numBeepsInput > 0) {
        count++;
        // Vibrating speaker.
        pio_output_toggle(PIEZO1_PIO);
        pio_output_toggle(PIEZO2_PIO);

        if (count >= CYCLE_COUNT) {
            count = 0;
            numBeepsInput--;

            if (numBeepsInput > 0) {
                shouldWait = true;
            }
        }
    }
}
