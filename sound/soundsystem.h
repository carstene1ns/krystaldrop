#ifndef SoundSystemH
#define SoundSystemH

#include "SDL.h"

/**
	Static class used to instanciate the Sound subsystem of SDL, and other sound-related stuff.
  */
class KD_SoundSystem
{
private:
	static SDL_AudioSpec *hardware_spec;

public:
	/**
		Instanciate the sound-system. Takes in parameters the frequency, the number of bits (8 or 16), if we want stereo sound.
		It also can optionally take the audio_buffer size. The size of the audio_buffer should be a power of 2 and no more than 8192.
		The bigger, the less we will have risks of buffer underflow, but we will also increase the latency of the sound.
	*/
	static signed initSoundSystem(int freq, int bits, bool stereo, int audio_buffer=4096);

	/**
		Close the sound system.
	*/
	static void deInit();
};

#endif
