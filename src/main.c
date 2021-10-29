#include "main.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	memset(&app, 0, sizeof(App));

	initSDL();

	atexit(cleanup);

	initStage();

	while (1)
	{
		prepareScene();

		doInput();

		app.delegate.logic();
		app.delegate.draw();

		presentScene();

		SDL_Delay(GAME_LOOP_DELAY);
	}

	return 0;
}