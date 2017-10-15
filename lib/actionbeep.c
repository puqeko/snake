// actionbeep.c
// Action beep module for initialising and playing a series of configurable beeps 
//
// Written by Jozef and Thomas
// Created 15/10/2017

#include "actionbeep.h"
#include "pio.h"
#include "pacer.h"

void beeper_init(void) 
// Initialise pacer and PIO pins to which the piezo speaker is connected
{
    pacer_init(LOOP_RATE);
    pio_config_set(PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set(PIEZO2_PIO, PIO_OUTPUT_HIGH);
}

void run_beep_cycle(uint8_t maxNumberCycles) 
// Runs the beep for the configured sound frequency, number of cycles and the on/off frequency.
// The configurations are defined in actionbeep.h
{
    bool playingMode = true;
    uint8_t count = 0;
    uint8_t currCycleCount = 0;
    // Currently an annoyance where I can't use the macro CYCLE_COUNTS in if statements as I'd like
    uint8_t maxCycleCount = 44; // Using a manually defined variable in place of CYCLE_COUNTS atm
    
    while (currCycleCount < maxNumberCycles)
    {
        if (playingMode) {
            /*  should be CYCLE_COUNTS below */
            while(count < maxCycleCount) {
                pacer_wait();
                count++;
                pio_output_toggle(PIEZO1_PIO);
                pio_output_toggle(PIEZO2_PIO);
            }
            playingMode = false;
            count = 0;
        } else { /* should be CYCLE_COUNTS below */
            while (count < maxCycleCount) {
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

void run_action_beep(toneMode inputSetting) 
// Calls run_beep_cycle with the required number of beep cycles depending on the enum toneMode argument
{
    switch(inputSetting) {
        case TONE_PUSH_EVENT:
            run_beep_cycle(PUSH_NUM_CYCLES);
            break;
        case TONE_FOOD_EAT:
            run_beep_cycle(FOOD_NUM_CYCLES);
            break;
        case TONE_END_SOUND:
            run_end_tone();
            break;
    }
}