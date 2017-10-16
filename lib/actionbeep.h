// actionbeep.h
//
// Action beep module for initialising and playing a series of configurable beeps

// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Created 15/10/2017

#include "game.h"
#include "pio.h"

#ifndef GAME_ACTIONBEEP_H
#define GAME_ACTIONBEEP_H

#define TONE_FREQUENCY 440

typedef enum TONEMODE {
    TONE_PUSH_EVENT = 0,
    TONE_FOOD_EAT,
    TONE_END_SOUND
} ToneMode;

// Initialise PIO pins and pacer
void sound_init(void);

// Runs the beep for the configured sound frequency, number of cycles and the on/off frequency
// Old function based on paced loops
void sound_beep(ToneMode inputSetting);

// Runs the end tone
void sound_end_tone(void);

// New function based on the task scheduler. Much better.
void sound_update(State* state);

#endif
