#if !defined(TITLE_H_INIT)
#define TITLE_H_INIT

#include "draw.h"
#include "input.h"
#include "SDL2/SDL.h"

extern App app;

extern SDL_Texture* loadTexture(char* filename);
extern void doInput(void);
extern void drawText(int x, int y, int r, int g, int b, int align, char* format, ...);

#endif