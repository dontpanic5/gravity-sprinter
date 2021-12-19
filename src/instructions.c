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

	char text1[MAX_TEXT];
	char text2[MAX_TEXT];
	char text3[MAX_TEXT];
	char text4[MAX_TEXT];
	char text5[MAX_TEXT];
	char textRot[MAX_TEXT];
	char textFall[MAX_TEXT];
	char textSideways[MAX_TEXT];
	char textAllGood[MAX_TEXT];

	strcpy_s(text1, MAX_TEXT, "LAND GENTLY ON THE HOUSES TO SUCK BLOOD!");
	strcpy_s(text2, MAX_TEXT, "SUCKING BLOOD GIVES YOU BLOOD ENERGY!");

	strcpy_s(text3, MAX_TEXT, "USE UP KEY TO FLY.");
	strcpy_s(text4, MAX_TEXT, "LEFT AND RIGHT KEYS TO ROTATE.");

	strcpy_s(text5, MAX_TEXT, "WATCH BATTY'S EXPRESSION TO KNOW IF SHE MIGHT CRASH OR LAND SAFELY.");

	strcpy_s(textRot, MAX_TEXT, "TOO ROTATED");
	strcpy_s(textFall, MAX_TEXT, "FALLING TOO FAST");
	strcpy_s(textSideways, MAX_TEXT, "SIDEWAYS TOO FAST");
	strcpy_s(textAllGood, MAX_TEXT, "ALL GOOD");


	drawText(50, 50, 255, 0, 0, TEXT_LEFT, text1);
	drawText(50, 100, 255, 0, 0, TEXT_LEFT, text2);
	drawText(50, 200, 255, 0, 0, TEXT_LEFT, text3);
	drawText(50, 250, 255, 0, 0, TEXT_LEFT, text4);
	drawText(50, 350, 255, 0, 0, TEXT_LEFT, text5);

	drawText(50, 425, 255, 0, 0, TEXT_LEFT, textRot);
	drawText(50 + WIN_X / 5, 425, 255, 0, 0, TEXT_LEFT, textFall);
	drawText(50 + WIN_X / 5 * 2.5, 425, 255, 0, 0, TEXT_LEFT, textSideways);
	drawText(50 + WIN_X / 5 * 4, 425, 255, 0, 0, TEXT_LEFT, textAllGood);
	
	blit(battyScaredRot, 75, 475, 15 + riseFall, BATTY_SCALE, SDL_FLIP_NONE);
	blit(battyScaredVert, 100 + WIN_X / 5, 475 + riseFall * 1.75, 0, BATTY_SCALE, SDL_FLIP_NONE);
	blit(battyScaredHorz, 75 + WIN_X / 5 * 2.5 + riseFall * 2, 475, 0, BATTY_SCALE, SDL_FLIP_NONE);
	blit(battySmile, 75 + WIN_X / 5 * 4, 475, 0, BATTY_SCALE, SDL_FLIP_NONE);
}