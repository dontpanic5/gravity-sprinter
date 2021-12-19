#include "title.h"

static void logic(void);
static void draw(postProcess_t* pp, SDL_Rect* ppSrc);

static SDL_Texture* batty;
static SDL_Texture* battyCringe;
static SDL_Texture* battySmile;
static SDL_Texture* bgTexture;
static SDL_Texture* bg2Texture;
#define BATTY_SCALE .6
#define EXAMPLE_SCALE .3

static int startCounter = 0;

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
	if (battyCringe == NULL)
		battyCringe = loadTexture("gfx/bat_scared.png");
	if (battySmile == NULL)
		battySmile = loadTexture("gfx/bat.png");
	if (bgTexture == NULL)
		bgTexture = loadTexture("gfx/night-town-background-sky.png");
	if (bg2Texture == NULL)
		bg2Texture = loadTexture("gfx/night-town-background-forest.png");

	startCounter = 1;
}

static void logic()
{
	doInput();

	// TODO logic to move to instructions

	if (app.space && startCounter > 30)
		initStage();
	else if (app.t && startCounter > 30)
		initInstructions();

	startCounter++;
}

static void draw(postProcess_t* pp, SDL_Rect* ppSrc)
{
	*pp = NONE;

	drawBg(bgTexture, bg2Texture, NULL, 0);

	blit(batty, 25, 25, 0, (float) BATTY_SCALE, SDL_FLIP_NONE);

	char text3[MAX_TEXT];
	char text3[MAX_TEXT];

	strcpy_s(text3, MAX_TEXT, "PRESS SPACE TO PLAY!");

	drawText(WIN_X / 2.5, 400, 255, 0, 0, TEXT_LEFT, text3);

	blit(battyCringe, WIN_X / 2 - 200, 550, 0, EXAMPLE_SCALE, SDL_FLIP_NONE);
	blit(battySmile, WIN_X / 2 + 50, 550, 0, EXAMPLE_SCALE, SDL_FLIP_NONE);
}