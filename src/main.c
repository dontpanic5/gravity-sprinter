#include "main.h"

App app;

static long then;
static float frRemainder;

static void capFrameRate()
{
	long wait = 16 + frRemainder;
	frRemainder -= (int)frRemainder;
	long frameTime = SDL_GetTicks() - then;
	wait -= frameTime;
	if (wait < 1)
		wait = 1;
	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "wait: %f", wait);
	SDL_Delay(wait);
	frRemainder += 0.667;
	then = SDL_GetTicks();
}

#ifdef __EMSCRIPTEN__
EM_BOOL one_iter()
#else
void one_iter()
#endif // __EMSCRIPTEN__
{
	doInput();

	postProcess_t pp = NONE;
	SDL_Rect ppSrc;
	memset(&ppSrc, 0, sizeof(SDL_Rect));

	app.delegate.logic(&pp, &ppSrc);

	prepareScene(pp, ppSrc);

	app.delegate.draw();

	presentScene(pp, ppSrc);

	++app.tickCount;

	capFrameRate();

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

	then = SDL_GetTicks();
	frRemainder = 0;

#ifdef __EMSCRIPTEN__
	emscripten_request_animation_frame_loop(one_iter, 0);
#else
	while (1)
	{
		one_iter();
	}
#endif

	return 0;
}