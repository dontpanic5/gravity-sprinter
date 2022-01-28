#if !defined(INSTRUCTIONS_H_INIT)
#define INSTRUCTIONS_H_INIT

#include "draw.h"
#include "input.h"
#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

// #defines go here

extern App app;

extern void doInput(void);
extern void drawText(int x, int y, int r, int g, int b, int align, char* format, ...);

#endif