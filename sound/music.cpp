#include "music.h"

#include <SDL.h>
#include <assert.h>

#include "../util/logfile.h"

KD_Music *current_music;

void musicDone()
{
	assert(current_music);
	current_music->MusicDone();
	if (current_music->getAutoDestruct())
	{
		delete current_music;
		current_music=0;
	}
}

KD_Music::KD_Music()
{
	music = 0;
	loop=true;
	isPlaying=false;
	autoDestruct=false;
}

KD_Music::~KD_Music()
{

}

void KD_Music::setLoop(bool mustLoop)
{
	loop = mustLoop;
}

signed KD_Music::Load(char *fileName)
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

	return false;
}

void KD_Music::CloseMusic()
{
	if (music)
		Mix_FreeMusic(music);
	music=0;
}

void KD_Music::PlayMusic()
{
	assert(music);

	current_music = this;
	isPlaying=true;

	if (loop)
		Mix_PlayMusic(music, -1);
	else
		Mix_PlayMusic(music, 0);

	Mix_HookMusicFinished(musicDone);
}

void KD_Music::StopMusic()
{
	isPlaying=false;
	Mix_HaltMusic();
}

bool KD_Music::getIsPlaying()
{
	return isPlaying;
}

void KD_Music::MusicDone()
{
	isPlaying=false;
	onMusicDone();
}

void KD_Music::onMusicDone()
{

}

void KD_Music::setAutoDestruct(bool autoDestruct)
{
	this->autoDestruct=autoDestruct;
}

bool KD_Music::getAutoDestruct()
{
	return autoDestruct;
}