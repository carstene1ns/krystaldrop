#ifndef Sound_H
#define Sound_H

#include <SDL_mixer.h>

/**
	A sound.
  */
class KD_Sound
{
private:
	/**
		The channel on which the sound is played in SDL_mixer.
		Useful to stop the sound.
	*/
	int soundChannel;

	/**
		The sound itself.
	*/
	Mix_Chunk *sound;

	/**
		The volume of the music.
		Integer between 0 and 127.
	*/
	int volume;

public:
	KD_Sound();
	~KD_Sound();

	/**
		Loads the sound from file fileName.
	*/
	bool LoadSound(char *fileName);
	void UnloadSound();

	/**
		Plays the sound
	*/
	void PlaySound();

	/**
		Plays the sound with a panning going from 0 to 255.
		0: sound on the left
		255: sound on the right.
	*/
	void PlaySound(int panning);

	/**
		Warning, undefined behaviour if the sound has already finished.
		As much as possible, we should try to avoid using this method.
	*/
	void StopSound();

	/**
		Sets the volume for this sound. This must be called before making a call to PlaySound();
	*/
	void setVolume(int volume);
};

#endif
