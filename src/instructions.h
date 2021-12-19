#if !defined(INSTRUCTIONS_H_INIT)
#define INSTRUCTIONS_H_INIT

#include "draw.h"
#include "input.h"
#include "SDL2/SDL.h"

#define BATTY_SCALE .3

extern App app;

extern SDL_Texture* loadTexture(char* filename);
extern void doInput(void);
extern void drawText(int x, int y, int r, int g, int b, int align, char* format, ...);

#endif