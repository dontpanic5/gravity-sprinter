#include "instructions.h"

static void logic(void);
static void draw(postProcess_t* pp, SDL_Rect* ppSrc);

static SDL_Texture* battySmile;
static SDL_Texture* battyScaredRot;
static SDL_Texture* battyScaredVert;
static SDL_Texture* battyScaredHorz;
static SDL_Texture* bgTexture;
static SDL_Texture* bg2Texture;

static double riseFall = 0;
static int isRising = 0;

void initInstructions()
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	if (battySmile == NULL)
		battySmile = loadTexture("gfx/bat_smile.png");
	if (battyScaredRot == NULL)
		battyScaredRot = loadTexture("gfx/bat_scared_rot.png");
	if (battyScaredVert == NULL)
		battyScaredVert = loadTexture("gfx/bat_scared_vert_velo.png");
	if (battyScaredHorz == NULL)
		battyScaredHorz = loadTexture("gfx/bat_scared_horz_velo.png");
	if (bgTexture == NULL)
		bgTexture = loadTexture("gfx/night-town-background-sky.png");
	if (bg2Texture == NULL)
		bg2Texture = loadTexture("gfx/night-town-background-forest.png");
}

static void logic()
{
	doInput();

	if (app.space)
		initStage();

	riseFall = riseFall + (isRising ? -1.2 : 1.5);

	if (riseFall > 50)
		isRising = 1;
	else if (riseFall < 0)
		isRising = 0;
}

static void draw(postProcess_t* pp, SDL_Rect* ppSrc)
{
	*pp = NONE;

	drawBg(bgTexture, bg2Texture, NULL, 0);

	drawText(50, 50, 255, 0, 0, TEXT_LEFT, "LAND GENTLY ON THE HOUSES TO SUCK BLOOD!");
	drawText(50, 100, 255, 0, 0, TEXT_LEFT, "SUCKING BLOOD GIVES YOU BLOOD ENERGY!");
	drawText(50, 200, 255, 0, 0, TEXT_LEFT, "USE UP KEY TO FLY.");
	drawText(50, 250, 255, 0, 0, TEXT_LEFT, "LEFT AND RIGHT KEYS TO ROTATE.");
	drawText(50, 350, 255, 0, 0, TEXT_LEFT, "WATCH BATTY'S EXPRESSION TO KNOW IF SHE MIGHT CRASH OR LAND SAFELY.");

	drawText(50, 425, 255, 0, 0, TEXT_LEFT, "TOO ROTATED");
	drawText(50 + WIN_X / 5, 425, 255, 0, 0, TEXT_LEFT, "FALLING TOO FAST");
	drawText(50 + WIN_X / 5 * 2.5, 425, 255, 0, 0, TEXT_LEFT, "SIDEWAYS TOO FAST");
	drawText(50 + WIN_X / 5 * 4, 425, 255, 0, 0, TEXT_LEFT, "ALL GOOD");

	drawText(WIN_X - 50, 150, 255, 0, 0, TEXT_RIGHT, "PRESS SPACE TO PLAY!");
	
	blit(battyScaredRot, 75, 475, 15 + riseFall, BATTY_SCALE, SDL_FLIP_NONE);
	blit(battyScaredVert, 100 + WIN_X / 5, 475 + riseFall * 1.75, 0, BATTY_SCALE, SDL_FLIP_NONE);
	blit(battyScaredHorz, 75 + WIN_X / 5 * 2.5 + riseFall * 2, 475, 0, BATTY_SCALE, SDL_FLIP_NONE);
	blit(battySmile, 75 + WIN_X / 5 * 4, 475, 0, BATTY_SCALE, SDL_FLIP_NONE);
}