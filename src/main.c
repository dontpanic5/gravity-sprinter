#include "main.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	memset(&app, 0, sizeof(App));

#ifdef SPAWN_HOUSE
	app.lastHouse = SPAWN_HOUSE;
#endif // SPAWN_HOUSE

	initSDL();

	initFonts();

	atexit(cleanup);

	initSounds();

	loadMusic("audio/spooky-times-9726.mp3");

	playMusic();

	initTitle();

	while (1)
	{
		prepareScene();

		doInput();

		app.delegate.logic();
		app.delegate.draw();

		presentScene();

		SDL_Delay(GAME_LOOP_DELAY);

		++app.tickCount;
	}

	return 0;
}