// actionbeep.c
// Action beep module for initialising and playing a series of configurable beeps
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Created 15/10/2017

#include "actionbeep.h"
#include "pio.h"
#include "pacer.h"

static uint16_t numBeepsInput = 0;

void beeper_init(void)
// Initialise PIO pins to which the piezo speaker is connected
{
    pio_config_set(PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set(PIEZO2_PIO, PIO_OUTPUT_HIGH);
}

void run_beep_cycle(uint16_t maxNumberCycles)
// Runs the beep for the configured sound frequency, number of cycles and the on/off frequency.
// The configurations are defined in actionbeep.h
{
    bool playingMode = true;
    uint16_t count = 0;
    uint16_t currCycleCount = 0;

    while (currCycleCount < maxNumberCycles)
    {
        if (playingMode) {
            /*  should be CYCLE_COUNT below */
            while(count < CYCLE_COUNT) {
                pacer_wait();
                count++;
                pio_output_toggle(PIEZO1_PIO);
                pio_output_toggle(PIEZO2_PIO);
            }
            playingMode = false;
            count = 0;
        } else { /* should be CYCLE_COUNT below */
            while (count < CYCLE_COUNT) {
                pacer_wait();
                count++;
            }
            playingMode = true;
            count = 0;
        }
        currCycleCount++;
    }

}

void run_end_tone(void)
{
    // do some fancy pitch / volume changes if possible or time permitting
}

void run_action_beep(ToneMode inputSetting)
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
            run_end_tone();
            break;
    }
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
