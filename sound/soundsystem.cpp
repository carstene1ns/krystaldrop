#include "soundsystem.h"

#include <SDL_mixer.h>

#include "stdlib.h"
#include "../util/logfile.h"

#define KD_AUDIO_INVALID_BITS_NUMBER	-200
#define KD_AUDIO_INITIALISATION_FAILED	-201

SDL_AudioSpec *KD_SoundSystem::hardware_spec = 0;

/* Prototype of our callback function */
void my_audio_callback(void *userdata, Uint8 *stream, int len)
{


}

signed KD_SoundSystem::initSoundSystem(int freq, int bits, bool stereo, int audio_buffer)
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



	
	return 0;
}

void KD_SoundSystem::deInit()
{

	Mix_CloseAudio();

	if (hardware_spec)
		free(hardware_spec);

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
