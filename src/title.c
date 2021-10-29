#include "title.h"

static SDL_Texture* batty;

void initTitle()
{
	batty = loadTexture("gfx/batty.png");
}

static void logic()
{
	doInput();
}