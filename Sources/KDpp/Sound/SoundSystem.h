#ifndef SoundSystemH
#define SoundSystemH

#include <SDL/SDL.h>

#include "../Tools/defines.h"

/**
	Static class used to instantiate the Sound subsystem of SDL, and other sound-related stuff.
  */
class DllExport KD_SoundSystem
{
private:
	static SDL_AudioSpec *hardware_spec;

	/**
		False if the system has not been initialized.
		By default, is false.
		Activated by InitSoundSystem.
	*/
	static bool activateSound;

	/**
		Volume of the sound effects. Between 0 and 127.
	*/
	static int sfxVolume;

	/**
		Volume of the music. Between 0 and 127.
	*/
	static int musicVolume;

public:
	/**
		Instantiate the sound-system. Takes in parameters the frequency, the number of bits (8 or 16), if we want stereo sound.
		It also can optionally take the audio_buffer size. The size of the audio_buffer should be a power of 2 and no more than 8192.
		The bigger, the less we will have risks of buffer underflow, but we will also increase the latency of the sound.
	*/
	static signed InitSoundSystem(int freq, int bits, bool stereo, int audio_buffer=2048);

	/**
		Close the sound system.
	*/
	static void deInit();

	/**
		Sets the average music volume.
		This change won't take place immediately, so you should call a music->SetVolume to force un update.
	*/
	static void SetMusicVolume(int musicVolume);

	/**
		Sets the average music volume.
	*/
	static int GetMusicVolume();

	/**
		Sets the average sound effects volume.
		This change won't take place immediately, so you should call a sound->SetVolume on every sound to force an update.
		In practice, we don't mind, we just have to wait
	*/
	static void SetSoundVolume(int sfxVolume);

	/**
		Gets the average music volume.
	*/
	static int GetSoundVolume();

	/**
		Returns true if the sound system is initialized.
		False otherwise.
	*/
	static bool GetActivateSound();

};

#endif
