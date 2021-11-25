#include "main.h"

App app;

#ifdef __EMSCRIPTEN__
EM_BOOL one_iter()
#else
void one_iter()
#endif // __EMSCRIPTEN__

{
	prepareScene();

	doInput();

	app.delegate.logic();
	app.delegate.draw();

	presentScene();

	++app.tickCount;

#ifdef __EMSCRIPTEN__
	return EM_TRUE;
#endif // __EMSCRIPTEN__
}

#ifdef _WIN32
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
int main()
#endif
{
	memset(&app, 0, sizeof(App));

#ifdef SPAWN_HOUSE
	app.lastHouse = SPAWN_HOUSE;
#endif // SPAWN_HOUSE

	initSDL();

	initFonts();

	atexit(cleanup);

	initSounds();

	loadMusic("audio/spooky-times-9726.ogg");

	playMusic();

	initTitle();

#ifdef __EMSCRIPTEN__
	emscripten_request_animation_frame_loop(one_iter, 0);
#else
	while (1)
	{
		one_iter();

		SDL_Delay(GAME_LOOP_DELAY);
	}
#endif

	return 0;
}