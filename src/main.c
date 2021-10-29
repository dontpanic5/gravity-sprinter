#include "main.h"

// how many pixels are a meter
#define M_TO_P 77

// acceleration of gravity
#define GRAVITY_METERS 9.8

// tweak this to get a good terminal velocity.
// Since the bat is using its wings, it should be pretty low since the bat glides
#define DESIRED_TERMINAL_VELOCITY_METERS -5

#define GAME_LOOP_DELAY 16

// TODO would be cool to get a real number for this
#define FLAP_FORCE_METERS -20

// how many degrees of rotation are possible in a second
#define ROTATION_SPEED 90

// TODO actually get the time of each loop.
// use this in calculations, it's the fraction of a second the game loops at
const double GAME_LOOP_FRACTION = GAME_LOOP_DELAY / 1000.0;

const double GRAVITY = GRAVITY_METERS * M_TO_P;
const int FLAP_FORCE = FLAP_FORCE_METERS * M_TO_P;

// we work backwards to get the drag force instead of computing it. Too much work!
// this gives us a magic number that is our computed drag that gives us our desired terminal velocity.
double const MAGIC_DRAG_NUMBER = GRAVITY_METERS / DESIRED_TERMINAL_VELOCITY_METERS;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	const double ROTATION_TICK = ROTATION_SPEED * GAME_LOOP_FRACTION;

	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	while (1)
	{
		prepareScene();

		// track player's acceleration this frame. Always start with gravity
		DoubleVector playerAcceleration = { 0, GRAVITY };
		double playerDrag = player.velocity.y * MAGIC_DRAG_NUMBER;
		playerAcceleration.y += playerDrag;

		doInput();
		if (app.right)
			player.rotation += ROTATION_TICK;
		if (app.left)
			player.rotation -= ROTATION_TICK;
		// TODO at this point I might want to bound the possible rotation values

		if (app.up)
		{
			double rot = player.rotation;
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
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accel: X Calc: %f", sin(rot / 180));
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accel: X: %f Y: %f ROT: %f", playerAcceleration.x, playerAcceleration.y, rot);
		}

		player.velocity.x += playerAcceleration.x * GAME_LOOP_FRACTION;
		player.velocity.y += playerAcceleration.y * GAME_LOOP_FRACTION;

		player.pos.x += (int) round(player.velocity.x * GAME_LOOP_FRACTION);
		player.pos.y += (int) round(player.velocity.y * GAME_LOOP_FRACTION);

		// TODO check bounds of window

		blit(player.texture, player.pos.x, player.pos.y, player.rotation, .25);

		presentScene();

		SDL_Delay(GAME_LOOP_DELAY);
	}

	return 0;
}