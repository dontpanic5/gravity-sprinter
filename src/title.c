#include "title.h"

static void logic(void);
static void draw(postProcess_t* pp, SDL_Rect* ppSrc);

static SDL_Texture* batty;
static SDL_Texture* bgTexture;
static SDL_Texture* bg2Texture;
static SDL_Texture* logo;
#define BATTY_SCALE .6
#define EXAMPLE_SCALE .3

static int startCounter = 0;
static int gChangeCounter = 15;

#ifndef _WIN32
void strcpy_s(char* s, int rsize_t, char* s2)
{
	strcpy(s, s2);
}
#endif // !_WIN32


void initTitle()
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	app.camera.x = 0;
	app.camera.y = 0;

	if (batty == NULL)
		batty = loadTexture("gfx/batty.png");
	if (bgTexture == NULL)
		bgTexture = loadTexture("gfx/night-town-background-sky.png");
	if (bg2Texture == NULL)
		bg2Texture = loadTexture("gfx/night-town-background-forest.png");
	if (logo == NULL)
		logo = loadTexture("gfx/logo.png");

	startCounter = 1;
}

static void logic()
{
	doInput();

	if (app.space && startCounter > 30)
		initStage();
	else if (app.t && startCounter > 30)
		initInstructions();
	
	if (gChangeCounter < 15)
		gChangeCounter--;
	if (gChangeCounter == 0)
		gChangeCounter = 15;

	if (app.g && gChangeCounter == 15)
	{
		if (app.gravity < LAST_G)
			app.gravity++;
		else
			app.gravity = 0;
		gChangeCounter--;
	}

	startCounter++;
}

static void draw(postProcess_t* pp, SDL_Rect* ppSrc)
{
	*pp = NONE;

	drawBg(bgTexture, bg2Texture, NULL, 0);

	blit(batty, -25, 25, 0, (float)BATTY_SCALE, SDL_FLIP_NONE);

	drawText(WIN_X / 2.5, 400, 255, 0, 0, TEXT_LEFT, "PRESS SPACE TO PLAY!");
	drawText(WIN_X / 2.5, 450, 255, 0, 0, TEXT_LEFT, "PRESS T FOR INSTRUCTIONS.");

	char gravityStr[MAX_TEXT];
	switch (app.gravity)
	{
	case G_EARTH:
#ifdef _WIN32
		strcpy_s(gravityStr, sizeof("EARTH"), "EARTH");
#else
		strcpy(gravityStr, "EARTH");
#endif // _WIN32
		break;
	case G_MOON:
#ifdef _WIN32
		strcpy_s(gravityStr, sizeof("MOON"), "MOON");
#else
		strcpy(gravityStr, "MOON");
#endif
		break;
	default:
		*gravityStr = NULL;
	}
	drawText(WIN_X / 2.5, 550, 255, 0, 0, TEXT_LEFT, "GRAVITY: %s", gravityStr);
	drawText(WIN_X / 2.5, 600, 255, 0, 0, TEXT_LEFT, "PRESS G TO CHANGE GRAVITY");

	blit(logo, 400, 50, 0, 1, SDL_FLIP_NONE);
}