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
	const char* p_data;
	unsigned long size;

	if (KD_SoundSystem::GetActivateSound())
	{
		if (sound) UnloadSound();
		
		if (fileName.IsArchived())
		{
			KD_ArchiveManager::FetchResource (fileName.GetArchiveName(), fileName.GetPath(), &p_data, &size);
			sound = Mix_LoadWAV_RW(SDL_RWFromMem((void*)p_data, size), 0);
		}
		else
		{
			sound = Mix_LoadWAV(fileName.GetPath().c_str());
		}

		if (!sound)
		{
			printf("Warning! Unable to load file %s\n",fileName.GetPath().c_str());
			KD_LogFile::printf("Warning! Unable to load file %s\n",fileName.GetPath().c_str());
			return false;
		}
		assert(sound);

		if (fileName.IsArchived())
		{
			KD_ArchiveManager::FreeResource (fileName.GetArchiveName(), fileName.GetPath());
		}

		return true;
	}

	return false;
}

void KD_Sound::UnloadSound()
{
	if (KD_SoundSystem::GetActivateSound())
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
	if (KD_SoundSystem::GetActivateSound())
	{
		assert(sound);

		// Plays the sound
		soundChannel = Mix_PlayChannel(-1, sound, 0);

		// Sets the volume // # k: why after PlayChannel ?
		Mix_Volume(soundChannel, volume*KD_SoundSystem::GetSoundVolume()/128);
	}
}

void KD_Sound::PlaySound(int panning)
{
	if (KD_SoundSystem::GetActivateSound())
	{
		assert(sound);

		// Plays the sound
		soundChannel = Mix_PlayChannel(-1, sound, 0);

		// Sets the volume // # k: why after PlayChannel ?
		Mix_Volume(soundChannel, volume*KD_SoundSystem::GetSoundVolume()/128);

		Mix_SetPanning(soundChannel, panning, 255-panning);
	}
}

void KD_Sound::StopSound()
{
	if (KD_SoundSystem::GetActivateSound())
	{
		Mix_HaltChannel(soundChannel);
	}
}

void KD_Sound::SetVolume(int volume)
{
	this->volume = volume;
}

#endif
