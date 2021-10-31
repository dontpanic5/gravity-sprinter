#ifndef SOUND_H_INIT
#define SOUND_H_INIT

#include "SDL_mixer.h"
#include "logging.h"
#include "main.h"

extern void batLog(char* format, ...);

enum
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_WINGS
};

enum
{
	SND_PLAYER_LAUGH,
	SND_PLAYER_CRASH,
	SND_PLAYER_FLAP,
	SND_MAX
};

#endif // !SOUND_H_INIT