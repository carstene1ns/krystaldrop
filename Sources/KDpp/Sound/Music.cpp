#ifndef NO_MUSIC

#include <assert.h>
#include <SDL/SDL.h>

#include "Music.h"
#include "SoundSystem.h"
#include "../Tools/Logfile.h"

KD_Music *current_music;

void musicDone()
{
	assert(current_music);
	current_music->MusicDone();
	if (current_music->GetAutoDestruct())
	{
		delete current_music;
		current_music=0;
	}
}

KD_Music::KD_Music()
{
	music= 0;
	loop= true;
	isPlaying= false;
	autoDestruct= false;
	volume=127; // Maximum volume
}

KD_Music::~KD_Music()
{
	if (isPlaying) StopMusic();
	if (music)     CloseMusic();
}

void KD_Music::SetLoop(bool mustLoop)
{
	loop = mustLoop;
}

signed KD_Music::Load(const char *fileName)
{
	if (KD_SoundSystem::GetActivateSound())
	{
		if (isPlaying)
			StopMusic();

		if (music)
			CloseMusic();

		music = Mix_LoadMUS(fileName);

		if (!music)
		{
			printf("Warning! Unable to load file %s\n",fileName);
			KD_LogFile::printf("Warning! Unable to load file %s\n",fileName);
		}
		assert(music);
	}
	return 0;
}

void KD_Music::CloseMusic()
{
	if (music)
		Mix_FreeMusic(music);
	music=0;
}

void KD_Music::PlayMusic()
{
	if (KD_SoundSystem::GetActivateSound())
	{
		assert(music);

		current_music = this;
		isPlaying=true;

		Mix_VolumeMusic(volume*KD_SoundSystem::GetMusicVolume()/128);

		if (loop)
			Mix_PlayMusic(music, -1);
		else
			Mix_PlayMusic(music, 0);

		Mix_HookMusicFinished(musicDone);
	}
}

void KD_Music::StopMusic()
{
	if (KD_SoundSystem::GetActivateSound())
	{
		if (!isPlaying) return;
		isPlaying=false;
		Mix_HaltMusic();
	}
}

bool KD_Music::GetIsPlaying()
{
	return isPlaying;
}

void KD_Music::MusicDone()
{
	if (KD_SoundSystem::GetActivateSound())
	{
		isPlaying=false;
		OnMusicDone();
	}
}

void KD_Music::OnMusicDone()
{

}

void KD_Music::SetAutoDestruct(bool autoDestruct)
{
	this->autoDestruct=autoDestruct;
}

bool KD_Music::GetAutoDestruct()
{
	return autoDestruct;
}

void KD_Music::SetVolume(int volume)
{
	this->volume = volume;
		
	if (KD_SoundSystem::GetActivateSound())
	{
		if (isPlaying)
			Mix_VolumeMusic(volume*KD_SoundSystem::GetMusicVolume()/128);
	}
}

KD_Music* KD_Music::GetCurrentMusic()
{
	return current_music;
}

#endif
