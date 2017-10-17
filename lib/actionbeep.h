// actionbeep.h
//
// Action beep module for initialising and playing a series of configurable beeps.
// Currently configured for 440 Hz.
//
// By: Jozef Crosland jrc149
// Thomas Morrison tjm195
// Created 15/10/2017

#include "game.h"

#ifndef GAME_ACTIONBEEP_H
#define GAME_ACTIONBEEP_H

#define TONE_FREQUENCY 440

typedef enum TONE_MODE {
    TONE_PUSH_EVENT = 0,
    TONE_FOOD_EAT
} ToneMode;

// Initialise PIO pins and pacer
void sound_init(void);

// Runs the beep of the configured sound frequency. Chaning the ToneMode sets the
// number of beeps to play.
void sound_beep(ToneMode inputSetting);

// Vibrate the buzzer from a task. Allows for non-blocking sounds.
void sound_update(State* state);

#endif
