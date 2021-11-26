#include "title.h"

static void logic(void);
static void draw(void);

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

	if (app.space && startCounter > 30)
		initStage();

	startCounter++;
}

static void draw()
{
	drawBg(bgTexture, bg2Texture, NULL, 0);

	IntVector sz = { 0,0 };
	SDL_QueryTexture(batty, NULL, NULL, &sz.x, &sz.y);

	blit(batty, 25, 25, 0, (float) BATTY_SCALE, SDL_FLIP_NONE);

	char text1a[MAX_TEXT];
	char text1b[MAX_TEXT];
	char text2[MAX_TEXT];
	char text3[MAX_TEXT];
	char text4[MAX_TEXT];
	char text5[MAX_TEXT];
	char text6[MAX_TEXT];

	strcpy_s(text1a, MAX_TEXT, "LAND GENTLY ON THE HOUSES");
	strcpy_s(text1b, MAX_TEXT, "TO SUCK BLOOD!");
	strcpy_s(text2, MAX_TEXT, "SUCKING BLOOD GIVES YOU BLOOD ENERGY!");

	strcpy_s(text4, MAX_TEXT, "USE UP KEY TO FLY.");
	strcpy_s(text5, MAX_TEXT, "LEFT AND RIGHT KEYS TO ROTATE.");

	strcpy_s(text3, MAX_TEXT, "PRESS SPACE TO PLAY!");

	strcpy_s(text6, MAX_TEXT, "WATCH BATTY'S EXPRESSION TO KNOW IF SHE MIGHT CRASH OR LAND SAFELY.");

	drawText(WIN_X / 2.5, 50, 255, 0, 0, TEXT_LEFT, text1a);
	drawText(WIN_X / 2.5, 100, 255, 0, 0, TEXT_LEFT, text1b);
	drawText(WIN_X / 2.5, 150, 255, 0, 0, TEXT_LEFT, text2);

	drawText(WIN_X / 2.5, 250, 255, 0, 0, TEXT_LEFT, text4);
	drawText(WIN_X / 2.5, 300, 255, 0, 0, TEXT_LEFT, text5);

	drawText(WIN_X / 2.5, 400, 255, 0, 0, TEXT_LEFT, text3);

	drawText(50, 500, 255, 0, 0, TEXT_LEFT, text6);
	blit(battyCringe, WIN_X / 2 - 200, 550, 0, EXAMPLE_SCALE, SDL_FLIP_NONE);
	blit(battySmile, WIN_X / 2 + 50, 550, 0, EXAMPLE_SCALE, SDL_FLIP_NONE);
}