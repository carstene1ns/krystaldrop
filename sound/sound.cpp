#ifndef NO_SOUND

#include <assert.h>

#include "sound.h"
#include "soundsystem.h"
#include "../util/logfile.h"

KD_Sound::KD_Sound()
{
	soundChannel = -1;
	sound = 0;

	// Maximum volume
	volume=127;
}

KD_Sound::~KD_Sound()
{
	
}

bool KD_Sound::LoadSound(char *fileName)
{
	if (sound) UnloadSound();
	
	sound = Mix_LoadWAV(fileName);

	if (!sound)
	{
		printf("Warning! Unable to load file %s\n",fileName);
		KD_LogFile::printf("Warning! Unable to load file %s\n",fileName);
		return false;
	}
	assert(sound);

	return true;
}

void KD_Sound::UnloadSound()
{
	if (sound)
	{
		Mix_FreeChunk(sound);
		sound = 0;
	}
}

void KD_Sound::PlaySound()
{
	assert(sound);

	// Plays the sound
	soundChannel = Mix_PlayChannel(-1, sound, 0);

	// Sets the volume
	Mix_Volume(soundChannel, volume*KD_SoundSystem::getSoundVolume()/128);
}

void KD_Sound::PlaySound(int panning)
{
	assert(sound);

	// Plays the sound
	soundChannel = Mix_PlayChannel(-1, sound, 0);

	// Sets the volume
	Mix_Volume(soundChannel, volume*KD_SoundSystem::getSoundVolume()/128);

	Mix_SetPanning(soundChannel, panning, 255-panning);
}

void KD_Sound::StopSound()
{
	Mix_HaltChannel(soundChannel);
}

void KD_Sound::setVolume(int volume)
{
	this->volume = volume;
}

#endif