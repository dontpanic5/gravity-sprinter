#include "instructions.h"

static void logic(postProcess_t* pp, SDL_Rect* ppSrc);
static void draw();

// static SDL_Texture* declarations go here

static int startCounter;

void initInstructions()
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	// load textures here
	// if (battySmile == NULL)
	//	battySmile = loadTexture("gfx/bat_smile.png");

	startCounter = 1;
}

static void logic(postProcess_t *pp, SDL_Rect *ppSrc)
{
	*pp = NONE;

	doInput();

	if (app.t && startCounter > 30)
		initTitle();

	if (startCounter <= 30)
		startCounter++;
}

static void draw()
{
}