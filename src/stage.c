#include "stage.h"

const double ROTATION_TICK = ROTATION_SPEED * GAME_LOOP_FRACTION;

const double GRAVITY = GRAVITY_METERS * M_TO_P;
const int FLAP_FORCE = FLAP_FORCE_METERS * M_TO_P;

// we work backwards to get the drag force instead of computing it. Too much work!
// this gives us a magic number that is our computed drag that gives us our desired terminal velocity.
double const MAGIC_DRAG_NUMBER = GRAVITY_METERS / DESIRED_TERMINAL_VELOCITY_METERS;

static void logic(void);
static void draw(void);
static void initPlayer();
static void initHouses();
static void battyLogic(int colliding);
static int checkCollisions();
static int collision(SDL_Rect r1, SDL_Rect r2);
static void resetStage(void);

static Entity* player;
static Entity* houses[NUM_OF_HOUSES];

static int houseLandedOn;
static int suckedBlood;
static int crashed;
static int landedPause = 0;
static int launchingFromHouseCounter = 0;

static SDL_Texture* playerTexture;
static SDL_Texture* houseTexture;

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	playerTexture = loadTexture("gfx/bat.png");
	houseTexture = loadTexture("gfx/house.png");

	resetStage();

	initPlayer();

	initHouses();
}

static void resetStage()
{
	free(player);
	free(houses[0]);

	launchingFromHouseCounter = 0;
	crashed = 0;
}

static void initPlayer()
{
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));

	player->texture = playerTexture;
	player->pos.x = 100;
	player->pos.y = 100;

	player->energy = 300;
}

void initHouses()
{
	int h = 0;
	SDL_QueryTexture(houseTexture, NULL, NULL, NULL, &h);
	h = (int)(h * HOUSE_SCALE);

	houses[0] = malloc(sizeof(Entity));
	memset(houses[0], 0, sizeof(Entity));
	houses[0]->texture = houseTexture;
	houses[0]->pos.x = WIN_X / 2;
	houses[0]->pos.y = WIN_Y - h;
	// energy that is given to batty
	houses[0]->energy = 300;

	houses[1] = malloc(sizeof(Entity));
	memset(houses[1], 0, sizeof(Entity));
	houses[1]->texture = houseTexture;
	houses[1]->pos.x = 200;
	houses[1]->pos.y = WIN_Y - h;
	// energy that is given to batty
	houses[1]->energy = 300;
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
		if (landedPause < 60)
			return;
		if (!app.up)
			return;
		else
		{
			// batty is taking off, reset
			landedPause = 0;
			houseLandedOn = 0;
			suckedBlood = 0;
			launchingFromHouseCounter = 1; // set this to 1 so we know to count for a second
		}
	}

	if (crashed)
	{
		if (!app.space)
			return;
		else
		{
			initTitle();
		}
	}

	int c = checkCollisions();

	battyLogic(c);

	// TODO check bounds of window
}

static void battyLogic(int colliding)
{
	// track player's acceleration this frame. Always start with gravity
	DoubleVector playerAcceleration = { 0, GRAVITY };
	double playerDrag = player->velocity.y * MAGIC_DRAG_NUMBER;
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
		playerAcceleration.x -= sin(rot / 180) * FLAP_FORCE;
		playerAcceleration.y += cos(rot / 180) * FLAP_FORCE;
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accel: X Calc: %f", sin(rot / 180));
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accel: X: %f Y: %f ROT: %f", playerAcceleration.x, playerAcceleration.y, rot);

		player->energy--;
	}

	player->velocity.x += playerAcceleration.x * GAME_LOOP_FRACTION;

	// this is our hacky way of not falling through houses after we land on them
	double yAccel = playerAcceleration.y * GAME_LOOP_FRACTION;
	if (!colliding || !launchingFromHouseCounter || yAccel < 0)
		player->velocity.y += yAccel;

	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "velocity x: %f y: %f", player->velocity.x, player->velocity.y);

	player->pos.x += (int)round(player->velocity.x * GAME_LOOP_FRACTION);

	double yVelo = (int)round(player->velocity.y * GAME_LOOP_FRACTION);
	if (!colliding || !launchingFromHouseCounter || yVelo < 0)
	{
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "yAccel %f yVelo: %f colliding: %d launchingFromHouseCounter", yAccel, yVelo, colliding, launchingFromHouseCounter);
		player->pos.y += yVelo;
	}
}

static int checkCollisions()
{
	int c = 0;

	SDL_Rect battyRect = { player->pos.x, player->pos.y, 0, 0 };
	SDL_QueryTexture(player->texture, NULL, NULL, &battyRect.w, &battyRect.h);
	battyRect.w *= BAT_SCALE;
	battyRect.h *= BAT_SCALE;

	int houseW = 0, houseH = 0;
	SDL_QueryTexture(houseTexture, NULL, NULL, &houseW, &houseH);
	houseW *= HOUSE_SCALE;
	houseH *= HOUSE_SCALE;

	SDL_Rect houseRects[NUM_OF_HOUSES];
	for (int i = 0; i < NUM_OF_HOUSES; i++)
	{
		houseRects[i].x = houses[i]->pos.x;
		houseRects[i].y = houses[i]->pos.y;
		houseRects[i].w = houseW;
		houseRects[i].h = houseH;

		int thisC = collision(battyRect, houseRects[i]);
		if (thisC)
		{
			if (player->velocity.y > SAFE_VELOCITY || player->velocity.x > SAFE_VELOCITY || abs(player->rotation) > SAFE_ROTATION)
			{
				crashed = 1;
			}
			else if (!launchingFromHouseCounter)
			{
				houseLandedOn = 1;
				if (houses[i]->energy > 0)
				{
					player->energy += houses[0]->energy;
					houses[i]->energy = 0;
					suckedBlood = 1;
				}
			}
		}

		if (!c)
			c = thisC;
	}
	
	return c;
}

static int collision(SDL_Rect r1, SDL_Rect r2)
{
	return (max(r1.x, r2.x) < min(r1.x + r1.w, r2.x + r2.w)) && (max(r1.y, r2.y) < min(r1.y + r1.h, r2.y + r2.h));
}

static void draw()
{
	for (int i = 0; i < NUM_OF_HOUSES; i++)
	{
		int h = 0;
		SDL_QueryTexture(houseTexture, NULL, NULL, NULL, &h);
		h = (int)(h * HOUSE_SCALE);
		blit(houseTexture, houses[i]->pos.x, houses[i]->pos.y, 0, HOUSE_SCALE);
	}

	

	blit(player->texture, player->pos.x, player->pos.y, player->rotation, BAT_SCALE);

	drawText(25, 25, 255, 0, 0, TEXT_LEFT, "BLOOD ENERGY: %d", player->energy);

	if (houseLandedOn)
		if (suckedBlood)
			drawText(WIN_X / 2, WIN_Y / 2, 255, 0, 0, TEXT_CENTER, "YOU LANDED SAFELY AND SUCKED SOME BLOOD.");
		else
			drawText(WIN_X / 2, WIN_Y / 2, 255, 0, 0, TEXT_CENTER, "YOU LANDED SAFELY BUT THERE IS NO MORE BLOOD TO SUCK HERE.");

	if (crashed)
	{
		drawText(WIN_X / 2, WIN_Y / 2, 255, 0, 0, TEXT_CENTER, "YOU DIDN'T LAND SAFELY ON THE HOUSE! YOU CRASHED!");
		drawText(WIN_X / 2, WIN_Y / 2 + 50, 255, 0, 0, TEXT_CENTER, "PRESS SPACE TO CONTINUE.");
	}
}