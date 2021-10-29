#ifndef STAGE_H_INIT
#define STAGE_H_INIT

#include "structs.h"
#include "main.h"

// how many pixels are a meter
#define M_TO_P 77

// acceleration of gravity
#define GRAVITY_METERS 9.8

// tweak this to get a good terminal velocity.
// Since the bat is using its wings, it should be pretty low since the bat glides
#define DESIRED_TERMINAL_VELOCITY_METERS -5

// TODO would be cool to get a real number for this
#define FLAP_FORCE_METERS -20

// how many degrees of rotation are possible in a second
#define ROTATION_SPEED 90

// TODO actually get the time of each loop.
// use this in calculations, it's the fraction of a second the game loops at
#define GAME_LOOP_FRACTION 0.016

#endif // !STAGE_H_INIT