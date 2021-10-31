#include "title.h"

static void logic(void);
static void draw(void);

static SDL_Texture* batty;
#define BATTY_SCALE .6

static int startCounter = 0;

void initTitle()
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	app.camera.x = 0;
	app.camera.y = 0;

	batty = loadTexture("gfx/batty.png");

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
	IntVector sz = { 0,0 };
	SDL_QueryTexture(batty, NULL, NULL, &sz.x, &sz.y);

	int posX = (int) (WIN_X - (sz.x * BATTY_SCALE)) / 2;

	blit(batty, 25, 25, 0, (float) BATTY_SCALE);

	char text1a[MAX_TEXT];
	char text1b[MAX_TEXT];
	char text2[MAX_TEXT];
	char text3[MAX_TEXT];
	char text4[MAX_TEXT];
	char text5[MAX_TEXT];

	strcpy_s(text1a, MAX_TEXT, "LAND SAFELY ON THE TOPS OF THE HOUSES");
	strcpy_s(text1b, MAX_TEXT, "TO SUCK BLOOD!");
	strcpy_s(text2, MAX_TEXT, "SUCKING BLOOD GIVES YOU BLOOD ENERGY!");

	strcpy_s(text4, MAX_TEXT, "USE UP KEY TO FLY.");
	strcpy_s(text5, MAX_TEXT, "LEFT AND RIGHT KEYS TO ROTATE.");

	strcpy_s(text3, MAX_TEXT, "PRESS SPACE TO PLAY!");

	drawText(WIN_X / 2.5, 100, 255, 0, 0, TEXT_LEFT, text1a);
	drawText(WIN_X / 2.5, 150, 255, 0, 0, TEXT_LEFT, text1b);
	drawText(WIN_X / 2.5, 200, 255, 0, 0, TEXT_LEFT, text2);

	drawText(WIN_X / 2.5, 350, 255, 0, 0, TEXT_LEFT, text4);
	drawText(WIN_X / 2.5, 400, 255, 0, 0, TEXT_LEFT, text5);

	drawText(WIN_X / 2.5, 500, 255, 0, 0, TEXT_LEFT, text3);
}