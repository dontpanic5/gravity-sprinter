#ifndef SOUND_H_INIT
#define SOUND_H_INIT

#include "SDL_mixer.h"
#include "logging.h"
#include "main.h"

extern App app;

enum
{
	CH_ANY = -1,
	CH_PLAYER
};

enum
{
	SND_EXAMPLE,
	SND_MAX
};

#endif // !SOUND_H_INIT