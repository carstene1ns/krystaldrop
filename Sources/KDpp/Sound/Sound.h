#ifndef Sound_H
#define Sound_H

#include <SDL/SDL_mixer.h>

#include "../Tools/defines.h"
#include "../Tools/FilePath.h"
#include "../Resources/Resource.h"

/**
	A sound.
  */
class DllExport KD_Sound : public KD_Resource
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
	KD_Sound(const KD_FilePath &fileName);
	virtual ~KD_Sound();

	/**
		Loads the sound from file fileName.
	*/
	bool LoadSound(const KD_FilePath &fileName);
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
	void SetVolume(int volume);
};

#endif
