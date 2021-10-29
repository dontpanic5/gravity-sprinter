#include "title.h"

static void logic(void);
static void draw(void);

static SDL_Texture* batty;
#define BATTY_SCALE .6

void initTitle()
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	batty = loadTexture("gfx/batty.png");
}

static void logic()
{
	doInput();

	if (app.up)
		initStage();
}

static void draw()
{
	IntVector sz = { 0,0 };
	SDL_QueryTexture(batty, NULL, NULL, &sz.x, &sz.y);

	int posX = (int) (WIN_X - (sz.x * BATTY_SCALE)) / 2;

	blit(batty, posX, 25, 0, (float) BATTY_SCALE);

	char text1[MAX_TEXT];
	char text2[MAX_TEXT];
	char text3[MAX_TEXT];

	strcpy_s(text1, MAX_TEXT, "LAND SAFELY ON THE TOPS OF THE HOUSES TO SUCK BLOOD!");
	strcpy_s(text2, MAX_TEXT, "SUCKING BLOOD GIVES YOU BLOOD ENERGY!");
	strcpy_s(text3, MAX_TEXT, "PRESS UP TO PLAY!");

	drawText(WIN_X / 2, WIN_Y - 200, 255, 0, 0, TEXT_CENTER, text1);
	drawText(WIN_X / 2, WIN_Y - 150, 255, 0, 0, TEXT_CENTER, text2);
	drawText(WIN_X / 2, WIN_Y - 100, 255, 0, 0, TEXT_CENTER, text3);
}