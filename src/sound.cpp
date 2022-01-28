#include "sound.h"

static void loadSounds();

static Mix_Chunk* sounds[SND_MAX];
static Mix_Music* music;

void initSounds()
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

	music = NULL;

	loadSounds();
}

void loadMusic(char* filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	
	music = Mix_LoadMUS(filename);
	if (!music)
	{
		const char* err;
		err = Mix_GetError();
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "load music failed %s", err);
	}
}

void playMusic()
{
#ifdef NO_MUSIC
	return;
#endif // NO_MUSIC

	Mix_VolumeMusic(32);
	Mix_PlayMusic(music, -1);
}

void playSound(int id, int chan, int loop)
{
	Mix_PlayChannel(chan, sounds[id], loop);
}

void haltChannel(int chan)
{
	Mix_HaltChannel(chan);
}

static void loadSounds()
{
	//sounds[SND_EXAMPLE] = Mix_LoadWAV("audio/example");
}