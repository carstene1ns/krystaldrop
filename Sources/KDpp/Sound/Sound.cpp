#ifndef NO_SOUND

#include <assert.h>

#include "Sound.h"
#include "SoundSystem.h"
#include "../Tools/Logfile.h"

KD_Sound::KD_Sound() : KD_Resource()
{
	soundChannel = -1;
	sound = 0;

	// Maximum volume
	volume=127;
}

KD_Sound::KD_Sound(const KD_FilePath &fileName) : KD_Resource()
{
		LoadSound(fileName);
}

KD_Sound::~KD_Sound()
{
	
}

bool KD_Sound::LoadSound(const KD_FilePath &fileName)
{
	if (KD_SoundSystem::getActivateSound())
	{
		if (sound) UnloadSound();
		
		if (!fileName.IsArchived())
		{

			sound = Mix_LoadWAV(fileName.GetPath().c_str());

			if (!sound)
			{
				printf("Warning! Unable to load file %s\n",fileName.GetPath().c_str());
				KD_LogFile::printf("Warning! Unable to load file %s\n",fileName.GetPath().c_str());
				return false;
			}
			assert(sound);

			return true;
		}
	}
	return false;
}

void KD_Sound::UnloadSound()
{
	if (KD_SoundSystem::getActivateSound())
	{
		if (sound)
		{
			Mix_FreeChunk(sound);
			sound = 0;
		}
	}
}

void KD_Sound::PlaySound()
{
	if (KD_SoundSystem::getActivateSound())
	{
		assert(sound);

		// Plays the sound
		soundChannel = Mix_PlayChannel(-1, sound, 0);

		// Sets the volume
		Mix_Volume(soundChannel, volume*KD_SoundSystem::GetSoundVolume()/128);
	}
}

void KD_Sound::PlaySound(int panning)
{
	if (KD_SoundSystem::getActivateSound())
	{
		assert(sound);

		// Plays the sound
		soundChannel = Mix_PlayChannel(-1, sound, 0);

		// Sets the volume
		Mix_Volume(soundChannel, volume*KD_SoundSystem::GetSoundVolume()/128);

		Mix_SetPanning(soundChannel, panning, 255-panning);
	}
}

void KD_Sound::StopSound()
{
	if (KD_SoundSystem::getActivateSound())
	{
		Mix_HaltChannel(soundChannel);
	}
}

void KD_Sound::SetVolume(int volume)
{
	this->volume = volume;
}

#endif
