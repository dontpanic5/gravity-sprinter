#include "title.h"

static void logic(postProcess_t* pp, SDL_Rect* ppSrc);
static void draw();

static SDL_Texture* bgTexture;
static SDL_Texture* bg2Texture;
static SDL_Texture* logo;

static int startCounter;
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

	if (bgTexture == NULL)
		bgTexture = loadTexture("gfx/night-town-background-sky.png");
	if (bg2Texture == NULL)
		bg2Texture = loadTexture("gfx/night-town-background-forest.png");
	if (logo == NULL)
		logo = loadTexture("gfx/logo.png");

	startCounter = 1;
}

static void logic(postProcess_t* pp, SDL_Rect* ppSrc)
{
	*pp = NONE;

	doInput();

	if (app.space && startCounter > 30)
		initStage();
	else if (app.t && startCounter > 30)
		initInstructions();
	
	if (gChangeCounter < 15)
		gChangeCounter--;
	if (gChangeCounter == 0)
		gChangeCounter = 15;

	/* some action
	if (app.g && gChangeCounter == 15)
	{
		if (app.gravity < LAST_G)
			app.gravity++;
		else
			app.gravity = 0;
		gChangeCounter--;
	}
	*/

	if (startCounter <= 30)
		startCounter++;
}

static void draw()
{
	drawBg(bgTexture, bg2Texture, NULL, 0);

	blit(logo, 400, 50, 0, 1, SDL_FLIP_NONE);
}