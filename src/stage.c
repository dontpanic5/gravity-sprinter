#include "stage.h"

#define P1(x1, y1) { ++i; gp[i].x = gp[i - 1].x + x1; gp[i].y = gp[i - 1].y + y1; }
#define P4(x4, y4) { ++i4; gp4[i4].x = gp[i].x + x4; gp4[i4].y = gp[i].y + y4; }

const double ROTATION_TICK = ROTATION_SPEED * GAME_LOOP_FRACTION;

double gravity;
int flapForce;

// we work backwards to get the drag force instead of computing it. Too much work!
// this gives us a magic number that is our computed drag that gives us our desired terminal velocity.
double magicDragNumber;

const int TEXT_X = WIN_X / 2;
const int TEXT_Y = WIN_Y / 2 - 200;

static void logic(void);
static void draw(postProcess_t* pp, SDL_Rect* ppSrc);
static void initPlayer();
static void initLevel();
static void battyLogic(int colliding);
static int checkCollisions();
static void resetStage(void);
static void stopFlapping();
static void resetState();
static void resetPlayer(int energy);
static void drawMiniMap(void);

static Entity* player;
static Entity* houses[NUM_OF_HOUSES];

// groundPoints (made it two letters because I have to type it so much

static IntVector gp[NUM_OF_GROUND_POINTS];
static IntVector gp2[NUM_OF_GROUND_POINTS2];
static IntVector gp3[NUM_OF_GROUND_POINTS3];
static IntVector gp4[NUM_OF_GROUND_POINTS4];

static IntVector* allGp[TOT_NUM_LINES];
static int gpLengths[TOT_NUM_LINES];

static int houseLandedOn;
static int suckedBlood;
static int crashed;
static int outOfBounds = 0;
static int landedPause = 0;
static int launchingFromHouseCounter = 0;

static int slideDist = 0;

static int playingFlapSound = 0;
static int flap = 0;

static int battyFlipped = 0;

enum CRASH_TYPE {
	CRASH_ROT = 1,
	CRASH_VERT_VELO,
	CRASH_HORZ_VELO
};

static int houseHeight = -1;
static int houseWidth = -1;
static int battyHeight = -1;
static int battyWidth = -1;

static SDL_Texture* playerTexture;
static SDL_Texture* playerFlapTexture;
static SDL_Texture* chibiTexture;
static SDL_Texture* dedChibiTexture;
static SDL_Texture* playerScaredRotTexture;
static SDL_Texture* playerScaredRotFlapTexture;
static SDL_Texture* playerScaredVertVeloTexture;
static SDL_Texture* playerScaredVertVeloFlapTexture;
static SDL_Texture* playerScaredHorzVeloTexture;
static SDL_Texture* playerScaredHorzVeloFlapTexture;
static SDL_Texture* houseTexture;
static SDL_Texture* bgTexture;
static SDL_Texture* bg2Texture;
static SDL_Texture* bg3Texture;
static SDL_Texture* miniMapTexture;

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	switch (app.gravity)
	{
	case G_MOON:
		gravity = GRAVITY_METERS_MOON * M_TO_P;
		flapForce = FLAP_FORCE_METERS_MOON * M_TO_P;
		magicDragNumber = GRAVITY_METERS_MOON / DESIRED_TERMINAL_VELOCITY_METERS;
		break;
	case G_EARTH:
	default:
		gravity = GRAVITY_METERS * M_TO_P;
		flapForce = FLAP_FORCE_METERS * M_TO_P;
		magicDragNumber = GRAVITY_METERS / DESIRED_TERMINAL_VELOCITY_METERS;
	}

	if (playerTexture == NULL)
		playerTexture = loadTexture("gfx/bat_smile.png");
	if (playerFlapTexture == NULL)
		playerFlapTexture = loadTexture("gfx/bat_smile_flap.png");
	if (chibiTexture == NULL)
		chibiTexture = loadTexture("gfx/batty.png");
	if (dedChibiTexture == NULL)
		dedChibiTexture = loadTexture("gfx/ded_batty.png");
	if (playerScaredRotTexture == NULL)
		playerScaredRotTexture = loadTexture("gfx/bat_scared_rot.png");
	if (playerScaredRotFlapTexture == NULL)
		playerScaredRotFlapTexture = loadTexture("gfx/bat_scared_rot_flap.png");
	if (playerScaredVertVeloTexture == NULL)
		playerScaredVertVeloTexture = loadTexture("gfx/bat_scared_vert_velo.png");
	if (playerScaredVertVeloFlapTexture == NULL)
		playerScaredVertVeloFlapTexture = loadTexture("gfx/bat_scared_vert_velo_flap.png");
	if (playerScaredHorzVeloTexture == NULL)
		playerScaredHorzVeloTexture = loadTexture("gfx/bat_scared_horz_velo.png");
	if (playerScaredHorzVeloFlapTexture == NULL)
		playerScaredHorzVeloFlapTexture = loadTexture("gfx/bat_scared_horz_velo_flap.png");
	if (houseTexture == NULL)
		houseTexture = loadTexture("gfx/house.png");
	if (bgTexture == NULL)
		bgTexture = loadTexture("gfx/night-town-background-sky.png");
	if (bg2Texture == NULL)
		bg2Texture = loadTexture("gfx/night-town-background-forest.png");
	if (bg3Texture == NULL)
		bg3Texture = loadTexture("gfx/night-town-background-clouds.png");

	SDL_QueryTexture(houseTexture, NULL, NULL, &houseWidth, &houseHeight);
	houseWidth = (int)(houseWidth * HOUSE_SCALE);
	houseHeight = (int)(houseHeight * HOUSE_SCALE) - 15; // minus 15 to put it on the ground
	SDL_QueryTexture(playerTexture, NULL, NULL, &battyWidth, &battyHeight);
	battyHeight *= BATTY_HITBOX_Y_RATIO; // we don't want the bottom of the image because that's where her wings flap
	battyWidth = (int)(battyWidth * BAT_SCALE);
	battyHeight = (int)(battyHeight * BAT_SCALE);

	allGp[0] = &gp;
	allGp[1] = &gp2;
	allGp[2] = &gp3;
	allGp[3] = &gp4;

	gpLengths[0] = NUM_OF_GROUND_POINTS;
	gpLengths[1] = NUM_OF_GROUND_POINTS2;
	gpLengths[2] = NUM_OF_GROUND_POINTS3;
	gpLengths[3] = NUM_OF_GROUND_POINTS4;

	resetStage();

	initLevel();

	prepareMiniMap(&miniMapTexture);
	drawMiniMap();
	presentMiniMap(miniMapTexture);
	setRendToWin();

	initPlayer();
}

static void continueStage()
{
	resetState();

	resetPlayer(player->energy);
}

static void resetState()
{
	launchingFromHouseCounter = 0;
	crashed = 0;
	outOfBounds = 0;
}

static void resetStage()
{
	free(player);
	free(houses[0]);

	resetState();
}

static void setPlayerHitBox()
{
	player->hitbox[0].x = player->pos.x + (battyFlipped ? battyWidth - BATTY_HITBOX_LEFT_X * BAT_SCALE : BATTY_HITBOX_LEFT_X * BAT_SCALE);
	player->hitbox[0].y = player->pos.y + BATTY_HITBOX_TOP_Y * BAT_SCALE;
	player->hitbox[1].x = player->pos.x + (battyFlipped ? battyWidth - BATTY_HITBOX_RIGHT_X * BAT_SCALE : BATTY_HITBOX_RIGHT_X * BAT_SCALE);
	player->hitbox[1].y = player->pos.y + BATTY_HITBOX_TOP_Y * BAT_SCALE;
	player->hitbox[2].x = player->pos.x + (battyFlipped ? battyWidth - BATTY_HITBOX_RIGHT_X * BAT_SCALE : BATTY_HITBOX_RIGHT_X * BAT_SCALE);
	player->hitbox[2].y = player->pos.y + battyHeight;
	player->hitbox[3].x = player->pos.x + (battyFlipped ? battyWidth - BATTY_HITBOX_LEFT_X * BAT_SCALE : BATTY_HITBOX_LEFT_X * BAT_SCALE);
	player->hitbox[3].y = player->pos.y + battyHeight;

	IntVector center = {
		player->pos.x + 267 * BAT_SCALE,
		player->pos.y + 400 * BAT_SCALE
	};

	player->hitbox[0] = rotatePoint(player->hitbox[0], center, player->rotation);
	player->hitbox[1] = rotatePoint(player->hitbox[1], center, player->rotation);
	player->hitbox[2] = rotatePoint(player->hitbox[2], center, player->rotation);
	player->hitbox[3] = rotatePoint(player->hitbox[3], center, player->rotation);

}

static void resetPlayer(int energy)
{
	memset(&player->velocity, 0, sizeof(DoubleVector));
	player->rotation = 0;

	player->texture = playerTexture;

	if (app.lastHouse)
	{
		player->pos.x = houses[app.lastHouse]->pos.x;
		player->pos.y = houses[app.lastHouse]->pos.y - 300;
		app.camera.x = player->pos.x - WIN_X / 2;
		app.camera.y = player->pos.y - WIN_Y / 2 + 90;
	}
	else
	{
		player->pos.x = WIN_X / 2;
		player->pos.y = WIN_Y / 2;
		app.camera.x = 0;
		app.camera.y = 90;
	}

	setPlayerHitBox();

#ifdef MAX_ENERGY
	player->energy = 1000000;
#else// MAX_ENERGY
	player->energy = energy;
#endif
}

static void initPlayer()
{
	player = malloc(sizeof(Entity));

	resetPlayer(300 + BLOOD_MODIFIER);
}

void initHouse(int i, int x, int y, int energy)
{
	houses[i] = malloc(sizeof(Entity));
	memset(houses[i], 0, sizeof(Entity));
	houses[i]->texture = houseTexture;
	houses[i]->pos.x = x;
	houses[i]->pos.y = y - houseHeight;
	int hPosY = houses[i]->pos.y;
	houses[i]->hitbox[0].x = x + 0;
	houses[i]->hitbox[0].y = hPosY + HOUSE_HITBOX_P0 * HOUSE_SCALE;
	houses[i]->hitbox[1].x = x + HOUSE_HITBOX_P1 * HOUSE_SCALE;
	houses[i]->hitbox[1].y = hPosY;
	houses[i]->hitbox[2].x = x + HOUSE_HITBOX_P2 * HOUSE_SCALE;
	houses[i]->hitbox[2].y = hPosY;
	houses[i]->hitbox[3].x = x + houseWidth;
	houses[i]->hitbox[3].y = hPosY + HOUSE_HITBOX_P0;
	houses[i]->hitbox[4].x = x + houseWidth;
	houses[i]->hitbox[4].y = hPosY + houseHeight;
	houses[i]->hitbox[5].x = x;
	houses[i]->hitbox[5].y = hPosY + houseHeight;
	// energy that is given to batty
	houses[i]->energy = energy + BLOOD_MODIFIER;
}

static void initLevel()
{
	int i = 0;
	int h = 0;
	int i2 = 0;
	int i3 = 0;
	int i4 = 0;

	gp[i].x = 0;
	gp[i].y = -700;
	++i; // 1
	gp[i].x = 0;
	gp[i].y = WIN_Y;
	++i; // 2
	gp[i].x = 500;
	gp[i].y = WIN_Y;
	initHouse(h, 200, gp[i].y, 300);
	++i; // 3
	gp[i].x = 800;
	gp[i].y = gp[i - 1].y;
	++i; // 4
	gp[i].x = 1500;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, WIN_X - 200, gp[i].y, 300);
	++i; // 5
	gp[i].x = 1500;
	gp[i].y = 1400;
	++i; // 6
	gp[i].x = gp[i - 1].x + 700;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, 1600, 1400, 500);
	++i; // 7
	gp[i].x = gp[i - 1].x + 200;
	gp[i].y = gp[i - 1].y - 300;
	++i; // 8
	gp[i].x = gp[i - 1].x + 800;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[7].x + 300, gp[7].y, 200);
	++i; // 9
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y + 1000;
	++i; // 10
	gp[i].x = gp[i - 1].x + 300;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[i - 1].x + 50, gp[i].y, 300);
	++i; // 11
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y + 1500;
	++i; // 12
	gp[i].x = gp[i - 1].x + 300;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[i - 1].x + 50, gp[i].y, 100);
	++i; // 13
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y + 2000;
	++i; // 14
	gp[i].x = gp[i - 1].x + 300;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[i - 1].x + 50, gp[i].y, 100); // 6
	++i; // 15
	gp[i].x = gp[i - 1].x + 200;
	gp[i].y = gp[i - 1].y - 300;
	++i; // 16
	gp[i].x = gp[i - 1].x + 300;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[i - 1].x + 50, gp[i].y, 500); // 7
	++i; // 17
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y - 1000;
	++i; // 18
	gp[i].x = gp[i - 1].x + 300;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[i - 1].x + 50, gp[i].y, 500); // 8
	++i; // 19
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y - 1500;
	++i; // 20
	gp[i].x = gp[i - 1].x + 2800;
	gp[i].y = gp[i - 1].y;
	initHouse(++h, gp[i - 1].x + 500, gp[i].y, 500); // 9
	initHouse(++h, gp[i - 1].x + 2200, gp[i].y, 600); // 10

	gp2[i2].x = gp[19].x + 800;
	gp2[i2].y = gp[19].y - 300;
	++i2; // 1
	gp2[i2].x = gp2[i2 - 1].x + 1100;
	gp2[i2].y = gp2[i2 - 1].y;
	++i2; // 2
	gp2[i2].x = gp2[i2 - 1].x;
	gp2[i2].y = gp2[i2 - 1].y - 600;
	++i2; // 3
	gp2[i2].x = gp2[i2 - 1].x - 1100;
	gp2[i2].y = gp2[i2 - 1].y;
	++i2; // 4
	gp2[i2].x = gp2[i2 - 1].x;
	gp2[i2].y = gp2[i2 - 1].y + 600;

	++i; // 21
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y + 200;

	gp3[i3].x = gp[i].x;
	gp3[i3].y = gp[i].y - 300;

	++i; // 22
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y - 200;

	++i3; // 1
	gp3[i3].x = gp[i].x;
	gp3[i3].y = gp[i].y - 300;

	++i; //23
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y + 200;

	++i3; // 2
	gp3[i3].x = gp[i].x;
	gp3[i3].y = gp[i].y - 300;

	++i; // 24
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y - 200;

	++i3; // 3
	gp3[i3].x = gp[i].x;
	gp3[i3].y = gp[i].y - 300;

	++i; // 25
	gp[i].x = gp[i - 1].x + 400;
	gp[i].y = gp[i - 1].y + 200;

	++i3; // 4
	gp3[i3].x = gp[i].x;
	gp3[i3].y = gp[i].y - 300;
	++i3; // 5
	gp3[i3].x = gp3[i3 - 1].x;
	gp3[i3].y = gp3[i3 - 1].y - 1200;
	++i3; // 6
	gp3[i3].x = gp3[0].x;
	gp3[i3].y = gp3[i3 - 1].y;
	++i3; // 7
	gp3[i3].x = gp3[i3 - 1].x;
	gp3[i3].y = gp3[0].y;

	initHouse(++h, gp[i].x + 500, gp[i].y, 1500); // 11

	++i; // 26
	gp[i].x = gp[i - 1].x + 900;
	gp[i].y = gp[i - 1].y;
	P1(0, 2000); // 27

	gp4[i4].x = gp[i].x + 200;
	gp4[i4].y = gp[i].y - 3000;
	P4(200, -200); // 1
	P1(600, 0); // 28
	P4(-200, -200); // 2
	P1(0, -600); // 29
	P4(-200, -200); // 3
	P1(200, 0); // 30
	P4(200, -200); // 4
	P1(0, 400); // 31
	initHouse(++h, gp[i].x + 550, gp[i].y, 10000); // 12
	P4(200, -200); // 5
	++i4; // 6
	gp4[i4].x = gp4[i4 - 1].x + 200;
	gp4[i4].y = gp4[i4 - 1].y;
	++i4; // 7
	gp4[i4].x = gp4[i4 - 1].x;
	gp4[i4].y = gp4[i4 - 1].y - 300;
	++i4; // 8
	gp4[i4].x = gp4[i4 - 1].x + 500;
	gp4[i4].y = gp4[i4 - 1].y;
	P1(900, 0); // 32
	P4(0, 0); // 9
}

static void updateCamera()
{
	// calc overlaps by taking player pos and subtracting the location of the areas where we move the camera

	int rightOverlap = player->pos.x - (app.camera.x + WIN_X - CAM_BUF_HORZ);
	if (rightOverlap > 0)
	{
		//batLog("Moving camera to the right by %d", rightOverlap);
		app.camera.x += rightOverlap;
	}
	else
	{
		int leftOverlap = (app.camera.x + CAM_BUF_HORZ) - player->pos.x;
		if (leftOverlap > 0)
		{
			//batLog("Moving camera to the left by %d", leftOverlap);
			app.camera.x -= leftOverlap;
		}
	}

	int bottomOverlap = player->pos.y - (app.camera.y + WIN_Y - CAM_BUF_BOTTOM);
	if (bottomOverlap > 0)
	{
		//batLog("Moving camera down by %d", bottomOverlap);
		app.camera.y += bottomOverlap;
	}
	else
	{
		int topOverlap = (app.camera.y + CAM_BUF_TOP) - player->pos.y;
		if (topOverlap > 0)
		{
			//batLog("Moving camera up by %d", topOverlap);
			app.camera.y -= topOverlap;
		}
	}
}

static void logic(void)
{
	if (launchingFromHouseCounter && launchingFromHouseCounter < LAUNCH_COUNTER)
		launchingFromHouseCounter++;
	else if (launchingFromHouseCounter >= LAUNCH_COUNTER)
		launchingFromHouseCounter = 0;

	if (houseLandedOn)
	{
		landedPause++;
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "landed. Pause counter: %d up: %d", landedPause, app.up);
		//batLog("landed. Pause counter: %d up: %d", landedPause, app.up);
		if (landedPause < 60)
		{
			player->pos.x += (int)ceil(((double)slideDist) / 60);
			return;
		}
		if (!app.up)
			return;
		else
		{
			// batty is taking off, reset
			landedPause = 0;
			houseLandedOn = 0;
			suckedBlood = 0;
			launchingFromHouseCounter = 1; // set this to 1 so we know to count for a second
			slideDist = 0;
		}
	}

	if (crashed)
	{
		if (app.t)
		{
			app.lastHouse = 0;
			initTitle();
		}
		else if (player->energy > 0 && app.space)
		{
			continueStage();
		}
		return;
	}

	int c = checkCollisions();

	battyLogic(c);

	updateCamera();

	if (c || outOfBounds)
		stopFlapping();
}

void startFlapping()
{
	playSound(SND_PLAYER_FLAP, CH_WINGS, -1);
	playingFlapSound = 1;
	//batLog("FLAPPING");
}

void stopFlapping()
{
	haltChannel(CH_WINGS);
	playingFlapSound = 0;
	//batLog("STOP FLAPPING");
}

static void battyLogic(int colliding)
{
	// track player's acceleration this frame. Always start with gravity
	DoubleVector playerAcceleration = { 0, gravity };
	double playerDrag = player->velocity.y * magicDragNumber;
	playerAcceleration.y += playerDrag;

	if (app.right)
		player->rotation += ROTATION_TICK;
	if (app.left)
		player->rotation -= ROTATION_TICK;
	// TODO at this point I might want to bound the possible rotation values

	if (app.up && player->energy > 0)
	{
		double rot = player->rotation;
		if (rot > 90 && rot <= 180)
		{
			double diff = rot - 90;
			rot = 90 - diff;
		}
		// TODO I have the x accel working from 90 to 180 but the y is messed up in that range
		// TODO I also need to check negative rotations
		// subtract on the x axis
		playerAcceleration.x -= sin(rot / 180) * flapForce;
		playerAcceleration.y += cos(rot / 180) * flapForce;
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accel: X Calc: %f", sin(rot / 180));
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accel: X: %f Y: %f ROT: %f", playerAcceleration.x, playerAcceleration.y, rot);

		player->energy--;

		if (!playingFlapSound)
		{
			startFlapping();
		}
	}
	else {
		stopFlapping();
	}

	player->velocity.x += playerAcceleration.x * GAME_LOOP_FRACTION;

	double yAccel = playerAcceleration.y * GAME_LOOP_FRACTION;
	// this is our hacky way of not falling through houses after we land on them
	if (!colliding || !launchingFromHouseCounter || yAccel < 0)
	{
		player->velocity.y += yAccel;
	}

	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "velocity x: %f y: %f", player->velocity.x, player->velocity.y);

	player->pos.x += (int)round(player->velocity.x * GAME_LOOP_FRACTION);

	double yVelo = (int)round(player->velocity.y * GAME_LOOP_FRACTION);
	// this is our hacky way of not falling through houses after we land on them
	if (!colliding || !launchingFromHouseCounter || yVelo < 0)
	{
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "yAccel %f yVelo: %f colliding: %d launchingFromHouseCounter %d", yAccel, yVelo, colliding, launchingFromHouseCounter);
		player->pos.y += yVelo;
	}

	if (player->pos.y > LOWEST_ALLOWABLE || player->pos.x > RIGHTMOST_ALLOWABLE || player->pos.x < LEFTMOST_ALLOWABLE)
	{
		outOfBounds = 1;
		crashed = 1;
		playSound(SND_PLAYER_CRASH, CH_PLAYER, 0);
	}

	if (player->velocity.x > SAFE_VELOCITY)
		battyFlipped = 1;
	else if (player->velocity.x < -SAFE_VELOCITY)
		battyFlipped = 0;

	setPlayerHitBox();
}

int willCrash(int useCounter)
{
	if (fabs(player->rotation) > SAFE_ROTATION && (useCounter ? !launchingFromHouseCounter : 1))
		return CRASH_ROT;
	if (player->velocity.y > SAFE_VELOCITY)
		return CRASH_VERT_VELO;
	if (player->velocity.x > SAFE_VELOCITY)
		return CRASH_HORZ_VELO;

	return 0;
}

static int houseCollisions()
{
	int w = -1, h;
	SDL_QueryTexture(chibiTexture, NULL, NULL, &w, &h);
	w *= CHIBI_SCALE;
	h *= CHIBI_SCALE;

	int thisC = 0;
	// the boundaries of the house that comprise the hitbox
	for (int i = 0; i < NUM_OF_HOUSES; i++)
	{
		for (int h = 0; h < HOUSE_POINTS - 1; h++)
		{
			for (int b = 0; b < BATTY_POINTS - 1; b++)
			{
				int c = areIntersecting(houses[i]->hitbox[h], houses[i]->hitbox[h + 1], player->hitbox[b], player->hitbox[b + 1]);
				if (c)
				{
					thisC = c;
					break;
				}
			}
			if (thisC)
				break;
		}

		//if (i == 0)
		//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "player hb0: %d, %d player hb1x: %d player hb2y: %d player hb3x: %d house hb0: %d, %d hb3x: %d hb5y: %d", player->hitbox[0].x, player->hitbox[0].y, player->hitbox[1].x, player->hitbox[2].y, player->hitbox[3].x, houses[i]->hitbox[0].x, houses[i]->hitbox[0].y, houses[i]->hitbox[3].x, houses[i]->hitbox[5].y);
		// check for being inside house
		if (
			player->hitbox[0].x > houses[i]->hitbox[0].x && player->hitbox[1].x < houses[i]->hitbox[3].x &&
			player->hitbox[0].y > houses[i]->hitbox[1].y && player->hitbox[2].y < houses[i]->hitbox[5].y
			)
		{
			//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "inside house");
			thisC = 1;
		}

		if (thisC)
		{
			if (willCrash(1))
			{
				crashed = 1;
				playSound(SND_PLAYER_CRASH, CH_PLAYER, 0);
				//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "velo x: %f y: %f rot: %f", player->velocity.x, player->velocity.y, player->rotation);
				//batLog("velo x: %f y: %f rot: %f", player->velocity.x, player->velocity.y, player->rotation);

				if (player->energy >= 100)
					player->energy -= 100;
				else
					player->energy = 0;
			}
			else if (!launchingFromHouseCounter)
			{
				houseLandedOn = 1;
				app.lastHouse = i;
				if (houses[i]->energy > 0)
				{
					player->energy += houses[i]->energy;
					houses[i]->energy = 0;
					suckedBlood = 1;
					playSound(SND_PLAYER_LAUGH, CH_PLAYER, 0);
				}

				// check if Batty is hanging off the edge and fix

				if (player->pos.x + w > houses[i]->hitbox[3].x)
					slideDist = houses[i]->hitbox[3].x - (player->pos.x + w) - 50; // fudge it and subtract 100
				else if (player->pos.x < houses[i]->hitbox[0].x)
					slideDist = houses[i]->hitbox[0].x - player->pos.x;
			}
			return 1;
		}
	}
	return 0; // no collision
}


static int groundCollisions()
{
	for (int j = 0; j < TOT_NUM_LINES; j++)
	{
		for (int i = 0; i < gpLengths[j] - 1; i++)
		{
			for (int b = 0; b < BATTY_POINTS - 1; b++)
			{
				if (areIntersecting(player->hitbox[b], player->hitbox[b + 1], allGp[j][i], allGp[j][i + 1]))
				{
					crashed = 1;
					playSound(SND_PLAYER_CRASH, CH_PLAYER, 0);

					if (player->energy >= 100)
						player->energy -= 100;
					else
						player->energy = 0;
					return 1;
				}
			}
		}
	}
	return 0;
}

static int checkCollisions()
{
	return houseCollisions() || groundCollisions();
}

static int isBattyScared()
{
	for (int i = 0; i < NUM_OF_HOUSES; i++)
	{
		int x = abs(houses[i]->pos.x - player->pos.x);
		int y = abs(houses[i]->pos.y - player->pos.y);
		int crashType = willCrash(0);
		if (sqrt(x * x + y * y) < 400 && crashType)
		{
			return crashType;
		}
	}
	return 0;
}

static void drawBatty()
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (battyFlipped)
		flip = SDL_FLIP_HORIZONTAL;

	SDL_Texture* tex = NULL;

	if (crashed)
		blit(dedChibiTexture, player->pos.x, player->pos.y + 40, -90, CHIBI_SCALE, flip);
	else if (houseLandedOn)
		blit(chibiTexture, player->pos.x, player->pos.y, 0, CHIBI_SCALE, flip);
	else
	{
		int scaredVal = isBattyScared();
		if (playingFlapSound && app.tickCount % 8 < 4)
		{
			switch (scaredVal)
			{
			case CRASH_ROT:
				tex = playerScaredRotFlapTexture;
				break;
			case CRASH_VERT_VELO:
				tex = playerScaredHorzVeloFlapTexture;
				break;
			case CRASH_HORZ_VELO:
				tex = playerScaredVertVeloFlapTexture;
				break;
			case 0:
				tex = playerFlapTexture;
				break;
			}
			flap = 0;
		}
		else
		{
			switch (scaredVal)
			{
			case CRASH_ROT:
				tex = playerScaredRotTexture;
				break;
			case CRASH_VERT_VELO:
				tex = playerScaredVertVeloTexture;
				break;
			case CRASH_HORZ_VELO:
				tex = playerScaredHorzVeloTexture;
				break;
			case 0:
				tex = player->texture;
				break;
			}
			flap = 1;
		}
		blit(tex, player->pos.x, player->pos.y, player->rotation, BAT_SCALE, flip);
	}
#ifdef DRAW_HB
	drawLine(player->hitbox[0], player->hitbox[1]);
	drawLine(player->hitbox[1], player->hitbox[2]);
	drawLine(player->hitbox[2], player->hitbox[3]);
	drawLine(player->hitbox[3], player->hitbox[0]);
#endif // DRAW_HB

}

IntVector getMiniMapPoint(int x, int y)
{
	IntVector p = { ((double)x) / RIGHTMOST_ALLOWABLE * 290, ((double)y) / RIGHTMOST_ALLOWABLE * 290};
	return p;
}

static void drawMiniMap()
{
	for (int j = 0; j < TOT_NUM_LINES; j++)
		for (int i = 0; i < gpLengths[j] - 1; i++)
		{
			IntVector mini1 = getMiniMapPoint(allGp[j][i].x, allGp[j][i].y);
			IntVector mini2 = getMiniMapPoint(allGp[j][i + 1].x, allGp[j][i + 1].y);
			drawLine(mini1, mini2);
		}

	for (int i = 0; i < NUM_OF_HOUSES; i++)
	{
		IntVector p = getMiniMapPoint(houses[i]->pos.x, houses[i]->pos.y);
		drawLineColored(p, p, 255, 0, 0);
		p.x++;
		drawLineColored(p, p, 255, 0, 0);
		p.x++;
		drawLineColored(p, p, 255, 0, 0);
		p.y--;
		drawLineColored(p, p, 255, 0, 0);
		p.x--;
		drawLineColored(p, p, 255, 0, 0);
		p.x--;
		drawLineColored(p, p, 255, 0, 0);
		p.y--;
		drawLineColored(p, p, 255, 0, 0);
		p.x++;
		drawLineColored(p, p, 255, 0, 0);
		p.x++;
		drawLineColored(p, p, 255, 0, 0);
	}
}

static void draw(postProcess_t* pp, SDL_Rect* ppSrc)
{
	*pp = FACE_CAM;

	ppSrc->x = player->pos.x - app.camera.x;
	if (battyFlipped && !crashed)
		ppSrc->x += battyWidth - BATTY_FACE_W * BAT_SCALE;
	ppSrc->y = player->pos.y - app.camera.y + BATTY_FACE_Y * BAT_SCALE;
	if (houseLandedOn)
	{
		ppSrc->y -= 40;
		if (!battyFlipped)
			ppSrc->x += 20;
	}
	ppSrc->w = BATTY_FACE_W * BAT_SCALE;
	ppSrc->h = battyHeight - BATTY_FACE_Y * BAT_SCALE;

	drawBg(
		bgTexture,
		app.gravity == G_EARTH ? bg2Texture : NULL,
		app.gravity == G_EARTH ? bg3Texture : NULL,
		player->pos.x
	);


	SDL_Rect screen = {
		app.camera.x - SCREEN_BUFFER,
		app.camera.y - SCREEN_BUFFER,
		WIN_X + SCREEN_BUFFER * 2,
		WIN_Y + SCREEN_BUFFER * 2
	};

	for (int j = 0; j < TOT_NUM_LINES; j++)
		for (int i = 0; i < gpLengths[j] - 1; i++)
		{
			int x1 = allGp[j][i].x;
			int y1 = allGp[j][i].y;
			int x2 = allGp[j][i + 1].x;
			int y2 = allGp[j][i + 1].y;
			SDL_bool onScreen = SDL_IntersectRectAndLine(
				&screen,
				&x1,
				&y1,
				&x2,
				&y2
			);
			if (!onScreen)
				continue;

			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].y++;
			allGp[j][i + 1].y++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].x++;
			allGp[j][i + 1].x++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].y++;
			allGp[j][i + 1].y++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].x++;
			allGp[j][i + 1].x++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].y++;
			allGp[j][i + 1].y++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].x++;
			allGp[j][i + 1].x++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].y++;
			allGp[j][i + 1].y++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].x++;
			allGp[j][i + 1].x++;
			drawLine(allGp[j][i], allGp[j][i + 1]);
			allGp[j][i].y -= 4;
			allGp[j][i + 1].y -= 4;
			allGp[j][i].x -= 4;
			allGp[j][i + 1].x -= 4;
		}

	for (int i = 0; i < NUM_OF_HOUSES; i++)
	{
		blit(houseTexture, houses[i]->pos.x, houses[i]->pos.y, 0, HOUSE_SCALE, SDL_FLIP_NONE);
	}

	drawBatty();

	drawText(25, 25, 255, 0, 0, TEXT_LEFT, "BLOOD ENERGY: %d", player->energy);

	drawText(35, WIN_Y - 30, 255, 0, 0, TEXT_LEFT, "BATTY FACE CAM");

	IntVector miniMapTrans = { WIN_X - 280 + app.camera.x, 40 + app.camera.y };
	blit(miniMapTexture, miniMapTrans.x, miniMapTrans.y, 0, 1, SDL_FLIP_NONE);

	IntVector p = getMiniMapPoint(player->pos.x, player->pos.y);
	p.x += miniMapTrans.x;
	p.y += miniMapTrans.y;
	
	drawLineColored(p, p, 255, 0, 0);
	p.x++;
	drawLineColored(p, p, 255, 0, 0);
	p.y++;
	p.x--;
	drawLineColored(p, p, 255, 0, 0);
	p.x++;
	drawLineColored(p, p, 255, 0, 0);

	if (houseLandedOn)
	{
		if (suckedBlood)
		{
			drawText(TEXT_X, TEXT_Y, 255, 0, 0, TEXT_CENTER, "YOU LANDED SAFELY AND SUCKED SOME BLOOD!");
		}
		else
		{
			drawText(TEXT_X, TEXT_Y, 255, 0, 0, TEXT_CENTER, "YOU LANDED SAFELY BUT THERE IS NO MORE BLOOD TO SUCK HERE.");
		}
	}

	if (outOfBounds)
	{
		drawText(TEXT_X, TEXT_Y, 255, 0, 0, TEXT_CENTER, "YOU FLEW TOO FAR AWAY!");
		(player->energy > 0) ? drawText(TEXT_X, TEXT_Y + 50, 255, 0, 0, TEXT_CENTER, "PRESS SPACE TO CONTINUE.") : drawText(TEXT_X, TEXT_Y + 50, 255, 0, 0, TEXT_CENTER, "YOU ARE OUT OF BLOOD ENERGY!");
		drawText(TEXT_X, TEXT_Y + 100, 255, 0, 0, TEXT_CENTER, "PRESS T TO RETURN TO TITLE SCREEN.");
	}
	else if (crashed)
	{
		drawText(TEXT_X, TEXT_Y, 255, 0, 0, TEXT_CENTER, "YOU DIDN'T LAND SAFELY ON THE HOUSE! YOU CRASHED!");
		(player->energy > 0) ? drawText(TEXT_X, TEXT_Y + 50, 255, 0, 0, TEXT_CENTER, "PRESS SPACE TO CONTINUE.") : drawText(TEXT_X, TEXT_Y + 50, 255, 0, 0, TEXT_CENTER, "YOU ARE OUT OF BLOOD ENERGY!");
		drawText(TEXT_X, TEXT_Y + 100, 255, 0, 0, TEXT_CENTER, "PRESS T TO RETURN TO TITLE SCREEN.");
	}
}