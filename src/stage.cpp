#include "stage.h"
#include "StageDraw.h"

#define P1(x1, y1) { ++i; gp[i].x = gp[i - 1].x + x1; gp[i].y = gp[i - 1].y + y1; }
#define P2(x2, y2) { ++i2; gp2[i2].x = gp[i2].x + x2; gp2[i2].y = gp[i2].y + y2; }
#define P3(x3, y3) { ++i3; gp3[i3].x = gp[i3].x + x3; gp3[i3].y = gp[i3].y + y3; }
#define P4(x4, y4) { ++i4; gp4[i4].x = gp[i].x + x4; gp4[i4].y = gp[i].y + y4; }

constexpr int PLAYER_HEIGHT = 40;
constexpr int PLAYER_WIDTH = 20;

constexpr int NUMBER_OF_CHUNKS = 1;

const double ROTATION_TICK = ROTATION_SPEED * GAME_LOOP_FRACTION;

double gravity;
int flapForce;

// we work backwards to get the drag force instead of computing it. Too much work!
// this gives us a magic number that is our computed drag that gives us our desired terminal velocity.
double magicDragNumber;

const int TEXT_X = WIN_X / 2;
const int TEXT_Y = WIN_Y / 2 - 200;

static void logic(postProcess_t* pp, SDL_Rect* ppSrc);
static void draw(void);
static void initPlayer();
static void initLevel();
static void playerLogic();
static void checkCollisions(double* veloDelta, double* posDelta, bool* died);
static void resetStage(void);
static void stopFlapping();
static void resetState();
static void resetPlayer(int energy);
//static void drawMiniMap(void);

static Entity* player;

static LevelChunk allChunks[NUMBER_OF_CHUNKS];

static int currentChunkIdx = -1;
static int currentChunkStart = -1;
static int nextChunkIdx = -1;
static int nextChunkStart = -1;

static int crashed;

static int battyHeight = -1;
static int battyWidth = -1;

static int battyFlipped = 0;

// set textures

static SDL_Texture* playerTexture;

static int getRandomChunkIdx()
{
	return Rand<int>(NUMBER_OF_CHUNKS - 1, 0)();
}

static void setChunks()
{
	currentChunkIdx = getRandomChunkIdx();
	currentChunkStart = 0;
	nextChunkIdx = getRandomChunkIdx();
	nextChunkStart = allChunks[currentChunkIdx].getChunkLength();
}

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

	/*if (playerTexture == NULL)
		playerTexture = loadTexture("gfx/bat_smile.png");*/

	resetStage();

	initLevel();
	setChunks();

	/*prepareMiniMap(&miniMapTexture);
	drawMiniMap();
	presentMiniMap(miniMapTexture);*/
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
	crashed = 0;
}

static void resetStage()
{
	free(player);

	resetState();
}

static void setPlayerHitBox()
{
	player->hitbox[0].x = player->pos.x;
	player->hitbox[0].y = player->pos.y;
	player->hitbox[1].x = player->pos.x + PLAYER_WIDTH;
	player->hitbox[1].y = player->pos.y;
	player->hitbox[2].x = player->pos.x + PLAYER_WIDTH;
	player->hitbox[2].y = player->pos.y + PLAYER_HEIGHT;
	player->hitbox[3].x = player->pos.x;
	player->hitbox[3].y = player->pos.y + PLAYER_HEIGHT;

	/*IntVector center = {
		static_cast<int>(player->pos.x + 267 * BAT_SCALE),
		static_cast<int>(player->pos.y + 400 * BAT_SCALE)
	};

	player->hitbox[0] = rotatePoint(player->hitbox[0], center, player->rotation);
	player->hitbox[1] = rotatePoint(player->hitbox[1], center, player->rotation);
	player->hitbox[2] = rotatePoint(player->hitbox[2], center, player->rotation);
	player->hitbox[3] = rotatePoint(player->hitbox[3], center, player->rotation);*/

}

static void resetPlayer(int energy)
{
	memset(&player->velocity, 0, sizeof(DoubleVector));
	player->rotation = 0;

	player->texture = nullptr;

	// checkpoints
	if (app.checkpoint)
	{
		// set player to checkpoint

		app.camera.x = player->pos.x - WIN_X / 2;
		app.camera.y = player->pos.y - WIN_Y / 2 + 90;
	}
	else
	{
		player->pos.x = 150;
		player->pos.y = WIN_Y - PLAYER_HEIGHT;
		app.camera.x = 0;
		app.camera.y = 90;
	}

	player->velocity.x = SCROLL;

	setPlayerHitBox();
}

static void initPlayer()
{
	player = new Entity();

	resetPlayer(player->energy);
}

static void initLevel()
{
	int i = 0;
	int i2 = 0;
	int i3 = 0;
	int i4 = 0;

	IntVector gp[2];

	// set ground points

	gp[i].x = 0;
	gp[i].y = WIN_Y;
	++i; // 1
	gp[i].x = WIN_X * 2;
	gp[i].y = WIN_Y;


	allChunks[0].addLine({ 0, WIN_Y });
	allChunks[0].addPointAtOffset({ WIN_X * 2, 0 }, 0);


	//++i; // 2
	//gp[i].x = 500;
	//gp[i].y = WIN_Y;
	//++i; // 3
	//gp[i].x = 800;
	//gp[i].y = gp[i - 1].y;
	//++i; // 4
	//gp[i].x = 1500;
	//gp[i].y = gp[i - 1].y;
	//++i; // 5
	
	// any other lines

	//gp2[i2].x = gp[19].x + 800;
	//gp2[i2].y = gp[19].y - 300;
	++i2; // 1

	//P1(0, 2000); // 27

	//gp4[i4].x = gp[i].x + 200;
	//gp4[i4].y = gp[i].y - 3000;
	//P4(200, -200); // 1
}

static void updateCamera()
{
	app.camera.x += SCROLL;
}

static void logic(postProcess_t *pp, SDL_Rect *ppSrc)
{
	// turn this on for post processing
	*pp = NONE;

	ppSrc->x = player->pos.x - app.camera.x;
	double rotationTrans = player->rotation / -2;
	if (battyFlipped)
		rotationTrans *= -1;
	if (rotationTrans < 0)
		rotationTrans /= 2;
	ppSrc->y = 
		player->pos.y -
		app.camera.y +
		rotationTrans;

	// set some width and height for the post processing
	//ppSrc->w = BATTY_FACE_W * BAT_SCALE;
	//ppSrc->h = battyHeight - BATTY_FACE_Y * BAT_SCALE;

	if (crashed)
	{
		if (app.t)
		{
			app.checkpoint = 0;
			initTitle();
		}
		else if (player->energy > 0 && app.space)
		{
			continueStage();
		}
		return;
	}

	playerLogic();

	updateCamera();

	if (nextChunkStart < app.camera.x)
	{
		currentChunkStart = nextChunkStart;
		currentChunkIdx = nextChunkIdx;

		nextChunkIdx = getRandomChunkIdx();
		nextChunkStart = currentChunkStart + allChunks[currentChunkIdx].getChunkLength();
	}
}

// movement sound
void startFlapping()
{
	playSound(SND_EXAMPLE, CH_PLAYER, -1);
}

void stopFlapping()
{
	haltChannel(CH_PLAYER);
}

static void playerLogic()
{
	// track player's acceleration this frame. Always start with gravity
	DoubleVector playerAcceleration = { 0, gravity };
	double playerDrag = player->velocity.y * magicDragNumber;
	playerAcceleration.y += playerDrag;

	// jump / flap, optional energy requirement
	if (app.up /*&& player->energy > 0*/)
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
	}

	/*double yAccel = playerAcceleration.y * GAME_LOOP_FRACTION;
	player->velocity.y += yAccel;
	double yVelo = (int)round(player->velocity.y * GAME_LOOP_FRACTION);*/

	double veloDelta = playerAcceleration.y * GAME_LOOP_FRACTION;
	double posDelta = (int)round((player->velocity.y + veloDelta) * GAME_LOOP_FRACTION);

	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "velocity x: %f y: %f", player->velocity.x, player->velocity.y);

	bool died;

	checkCollisions(&veloDelta, &posDelta, &died);

	player->velocity.y += veloDelta;

	player->pos.y += posDelta;
	player->pos.x += SCROLL;

	// flipping logic
	/*if (player->velocity.x > SAFE_VELOCITY)
		battyFlipped = 1;
	else if (player->velocity.x < -SAFE_VELOCITY)
		battyFlipped = 0;*/

	setPlayerHitBox();
}

static void groundCollisions(double* veloDelta, double* posDelta, bool* died)
{
	for (int i = 0; i < allChunks[currentChunkIdx].getNumLines(); i++)
	{
		for (int j = 0; j < allChunks[currentChunkIdx].getLineLength(i) - 1; j++)
		{
			for (int p = 0; p < 3; p++)
			{
				IntVector h1 = player->hitbox[p];
				IntVector h2 = player->hitbox[p + 1];
				h1.y += static_cast<int>(*posDelta);
				h2.y += static_cast<int>(*posDelta);
				IntVector p1 = allChunks[currentChunkIdx].getPoint(i, j);
				IntVector p2 = allChunks[currentChunkIdx].getPoint(i, j + 1);
				p1.x += currentChunkStart;
				p2.x += currentChunkStart;
				if (areIntersecting(player->hitbox[p], player->hitbox[p + 1], p1, p2))
				{
					if (p1.x == p2.x)
					{
						*died = true;
						*veloDelta = 0;
						*posDelta = 0;
					}
					else
					{
						
					}
				}
			}
		}
	}
}

static void checkCollisions(double* veloDelta, double* posDelta, bool* died)
{
	groundCollisions(veloDelta, posDelta, died);
}

static void drawPlayer()
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (battyFlipped)
		flip = SDL_FLIP_HORIZONTAL;

	SDL_Texture* tex = NULL;

	if (crashed)
	{
		// draw crashed texture
	}
	else
	{
		// if animation condition
		//if (/*playingFlapSound &&*/ app.tickCount % 8 < 4)
		//{
		//	// choose animation sprite
		//}
		//else
		//{
		//	// choose animation sprite
		//}
		// draw animation sprite

		drawRect({ player->pos.x, player->pos.y, PLAYER_WIDTH, PLAYER_HEIGHT });
	}
#ifdef DRAW_HB
	drawLine(player->hitbox[0], player->hitbox[1]);
	drawLine(player->hitbox[1], player->hitbox[2]);
	drawLine(player->hitbox[2], player->hitbox[3]);
	drawLine(player->hitbox[3], player->hitbox[0]);
#endif // DRAW_HB

}

static void draw()
{
	/*drawBg(
		bgTexture,
		app.gravity == G_EARTH ? bg2Texture : NULL,
		app.gravity == G_EARTH ? bg3Texture : NULL,
		player->pos.x
	);*/


	SDL_Rect screen = {
		app.camera.x - SCREEN_BUFFER,
		app.camera.y - SCREEN_BUFFER,
		WIN_X + SCREEN_BUFFER * 2,
		WIN_Y + SCREEN_BUFFER * 2
	};

	LevelChunk &chunk = allChunks[currentChunkIdx];
	drawChunk(allChunks[currentChunkIdx], currentChunkStart);
	drawChunk(allChunks[nextChunkIdx], nextChunkStart);

	drawPlayer();

	/* draw minimap
	IntVector miniMapTrans = { WIN_X - 280 + app.camera.x, 40 + app.camera.y };
	blit(miniMapTexture, miniMapTrans.x, miniMapTrans.y, 0, 1, SDL_FLIP_NONE);

	IntVector p = getMiniMapPoint(player->pos.x, player->pos.y);
	p.x += miniMapTrans.x;
	p.y += miniMapTrans.y;

	SDL_Rect playerMini = { p.x - app.camera.x, p.y - app.camera.y, 3, 3 };
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(app.renderer, &playerMini);
	*/
}