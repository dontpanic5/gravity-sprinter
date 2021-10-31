#include "sound.h"

static void loadSounds();

static Mix_Chunk* sounds[SND_MAX];
static Mix_Music* music;

void initSounds()
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

	music = NULL;

	loadSounds();

	Mix_Volume(CH_WINGS, 96); // make wings quieter
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
		char* err;
		err = Mix_GetError();
		batLog("load music failed %s", err);
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
	sounds[SND_PLAYER_CRASH] = Mix_LoadWAV("audio/frustrated.ogg");
	sounds[SND_PLAYER_FLAP] = Mix_LoadWAV("audio/flap.wav");
	sounds[SND_PLAYER_LAUGH] = Mix_LoadWAV("audio/laugh.wav");
}