#if !defined(TEXT_H_INIT)
#define TEXT_H_INIT

#include <stdio.h>

#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH 18

#define MAX_TEXT 1024

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT,
};

extern void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
extern SDL_Texture* loadTexture(char* filename);

#endif