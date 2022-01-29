#ifndef STAGE_H_INIT
#define STAGE_H_INIT

#include "structs.h"
#include "main.h"
#include "text.h"
#include "sound.h"
#include "battyMath.h"
#include "Rand.h"
#include "chunkUtils.h"
#include <math.h>

// how many pixels are a meter
#define M_TO_P 77

// acceleration of gravity
#define GRAVITY_METERS 9.8
#define GRAVITY_METERS_MOON 1.62

// tweak this to get a good terminal velocity.
#define DESIRED_TERMINAL_VELOCITY_METERS -5
#define DESIRED_TERMINAL_VELOCITY_METERS_MOON -10

#define SCROLL 10

// how many degrees of rotation are possible in a second
#define ROTATION_SPEED 90

#define FLAP_FORCE_METERS -20
#define FLAP_FORCE_METERS_MOON -5

// TODO actually get the time of each loop.
// use this in calculations, it's the fraction of a second the game loops at
#define GAME_LOOP_FRACTION 0.016

#define SCREEN_BUFFER 100

// define hitbox parameters

// number of lines that make a batty hitbox
#define BATTY_POINTS 4
#define BATTY_HITBOX_LEFT_X 63
#define BATTY_HITBOX_TOP_Y 277
#define BATTY_HITBOX_RIGHT_X 474
#define BATTY_HITBOX_Y_RATIO .6
// number of lines that comprise a house hitbox
// the max is based on this right now
//#define HOUSE_POINTS 6
//#define HOUSE_HITBOX_P0 73
//#define HOUSE_HITBOX_P1 59
//#define HOUSE_HITBOX_P2 325

#define BAT_SCALE .20

#define LOWEST_ALLOWABLE 6500
#define LEFTMOST_ALLOWABLE -3000
#define RIGHTMOST_ALLOWABLE 17000

extern App app;
//extern postProcess_t;

extern void drawText(int x, int y, int r, int g, int b, int align, char* format, ...);
extern void drawLine(IntVector v1, IntVector v2);
extern void drawLineColored(IntVector v1, IntVector v2, Uint8 r, Uint8 g, Uint8 b);
extern void drawBg(SDL_Texture* bg, SDL_Texture* bg2, SDL_Texture* bg3, int pos);
extern void playSound(int id, int chan, int loop);
extern void haltChannel(int chan);
extern int areIntersecting(IntVector p1, IntVector q1, IntVector p2, IntVector q2);
extern IntVector rotatePoint(IntVector p, IntVector center, double deg);
extern void destroyTexture(SDL_Texture* texture);
extern void prepareMiniMap(SDL_Texture** tex);
extern void setRendToWin(void);
extern void presentMiniMap(SDL_Texture* tex);

#endif // !STAGE_H_INIT