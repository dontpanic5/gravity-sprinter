#if !defined(TITLE_H_INIT)
#define TITLE_H_INIT

#include "draw.h"
#include "input.h"
#include "SDL2/SDL.h"

extern SDL_Texture* loadTexture(char* filename);
extern void doInput(void);

#endif