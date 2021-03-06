#if !defined(DRAW_H_INCLUDED)
#define DRAW_H_INCLUDED

#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include "main.h"

extern App app;

SDL_Texture* loadTexture(const char* filename);

void drawRect(SDL_Rect rect, int r = 0, int g = 0, int b = 0);

#endif