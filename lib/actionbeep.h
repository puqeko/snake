// actionbeep.h
//
// Action beep module for initialising and playing a series of configurable beeps
// Mainly based on the beep1.c as provided from the 260 labs repository, but was easier to
// write a modified module.
// Credits still go to M.P Hayes and the ENCE260 team
//
// Modified by Jozef and Thomas
// Created 15/10/2017

#include "game.h"
#include "pio.h"

#ifndef GAME_ACTIONBEEP_H
#define GAME_ACTIONBEEP_H

typedef enum TONEMODE {
    TONE_PUSH_EVENT = 0,
    TONE_FOOD_EAT,
    TONE_END_SOUND
} toneMode;

//Connect piezo tweeter to pins 5 and 8 of UCFK4 P1 connector for push-pull operation.
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 1) // changed from 4 so that the piezo speaker would fit
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 5) // DON'T USE 6! Causes the game to fail to transition title>snake

#define TONE_FREQUENCY 440
#define LOOP_RATE (TONE_FREQUENCY * 2)
#define CYCLE_PERIOD 100 // desired cycle period in milliseconds
#define CYCLE_COUNT (uint16_t)((CYCLE_PERIOD / 1000.0) * LOOP_RATE) // number of times pacer_wait() is called
#define FOOD_NUM_CYCLES 5 // Note: this is the on and off periods combined!
#define PUSH_NUM_CYCLES 1 //                ^ as above ^


// Initialise PIO pins and pacer
void beeper_init(void);

// Runs the beep for the configured sound frequency, number of cycles and the on/off frequency.
void run_beep_cycle(uint16_t maxNumberCycles);

// Runs the end tone
void run_end_tone(void);

// Runs the beep for the configured sound frequency, number of cycles and the on/off frequency
void run_action_beep(toneMode inputSetting);

#endif
