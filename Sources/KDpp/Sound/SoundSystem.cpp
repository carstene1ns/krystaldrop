#ifndef NO_SOUND

#include <stdlib.h>
#include <SDL/SDL_mixer.h>

#include "SoundSystem.h"
#include "../Tools/Logfile.h"

#define KD_AUDIO_INVALID_BITS_NUMBER	-200
#define KD_AUDIO_INITIALISATION_FAILED	-201

SDL_AudioSpec *KD_SoundSystem::hardware_spec = 0;

bool KD_SoundSystem::activateSound = false;

int KD_SoundSystem::sfxVolume = 127;

int KD_SoundSystem::musicVolume = 127;

/* Prototype of our callback function */
/*
void my_audio_callback(void *userdata, Uint8 *stream, int len)
{ }
*/

signed KD_SoundSystem::InitSoundSystem(int freq, int bits, bool stereo, int audio_buffer)
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	
	Uint16 audio_format;
	/* Let's go for signed audio */
	if (bits==16)
		audio_format=AUDIO_S16LSB;
	else if (bits==8)
		audio_format=AUDIO_S8;
	else
		return KD_AUDIO_INVALID_BITS_NUMBER;

	int audio_channels;

	if (stereo==true)
		audio_channels=2;
	else
		audio_channels=1;

	if(Mix_OpenAudio(freq, audio_format, audio_channels, audio_buffer))
	{
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		printf("Couldn't open audio: %s\n", SDL_GetError());
		KD_LogFile::printf("Couldn't open audio: %s\n", SDL_GetError());
		return KD_AUDIO_INITIALISATION_FAILED;
	}

	Uint16 audio_format_obt;
	int freq_obt, audio_channels_obt;
	Mix_QuerySpec(&freq_obt, &audio_format_obt, &audio_channels_obt);

	if (freq!=freq_obt || audio_format!=audio_format_obt || audio_channels!=audio_channels_obt)
	{
		printf("Audio system warning: Asked an audio device with %d Hz, %d audio channels, and audio-format %d\n   Got an audio device with %d Hz, %d audio channels, and audio-format %d\n", freq, audio_channels, audio_format, freq_obt, audio_channels_obt, audio_format_obt);
		KD_LogFile::printf("Audio system warning: Asked an audio device with %d Hz, %d audio channels, and audio-format %d\n   Got an audio device with %d Hz, %d audio channels, and audio-format %d\n", freq, audio_channels, audio_format, freq_obt, audio_channels_obt, audio_format_obt);
	}

	activateSound = true;

	return 0;
}

void KD_SoundSystem::deInit()
{
	if (activateSound)
	{
		Mix_CloseAudio();

		if (hardware_spec)
			free(hardware_spec);

		SDL_QuitSubSystem(SDL_INIT_AUDIO);

		activateSound=false;
	}
}

void KD_SoundSystem::SetMusicVolume(int musicVolume)
{
	if (musicVolume<0) musicVolume=0;
	if (musicVolume>127) musicVolume=127;

	KD_SoundSystem::musicVolume = musicVolume;
}

int KD_SoundSystem::GetMusicVolume()
{
	return musicVolume;
}

void KD_SoundSystem::SetSoundVolume(int sfxVolume)
{
	if (musicVolume<0) sfxVolume=0;
	if (musicVolume>127) sfxVolume=127;

	KD_SoundSystem::sfxVolume = sfxVolume;
}

int KD_SoundSystem::GetSoundVolume()
{
	return sfxVolume;
}

bool KD_SoundSystem::GetActivateSound()
{
	return activateSound;
}

#endif
