#if !defined(INIT_H_INCLUDED)
#define INIT_H_INCLUDED

#include <stdio.h>
#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "main.h"
#include "logging.h"
#include "sound.h"

extern App app;

extern const IntVector windowSz;
extern void batLog(char* format, ...);
extern void initSounds();
extern void createRendTex();

#endif