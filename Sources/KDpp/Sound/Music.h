#ifndef Music_H
#define Music_H

#include <SDL/SDL_mixer.h>

#include "../Tools/defines.h"

/**
	A music.
  */
class DllExport KD_Music
{
private:
	Mix_Music *music;

	/**
		True if the music must loop, false if it mustn't.
	*/
	bool loop;

	/**
		True if the music is played, false otherwise.
	*/
	bool isPlaying;

	/**
		If true, the music will auto destruct on end of the music.
	*/
	bool autoDestruct;

	/**
		The volume of the music.
		Integer between 0 and 127.
	*/
	int volume;

public:
	KD_Music();
	virtual ~KD_Music();

	/**
		if true, then the music will loop undefinitely.
		if false, it won't loop.
		This method must be called before the call to PlayMusic is performed.
	*/
	void SetLoop(bool mustLoop);

	signed Load(const char *fileName);
	void CloseMusic();

	
	void PlayMusic();
	void StopMusic();

	bool GetIsPlaying();

	/**
		Function called by the global callback used in SDL_mixer.
		This function should never be called by the user.
	*/
	void MusicDone();

	/**
		Virtual function not doing anything. It can be overloaded in a subclass to provide some actions when a music finishes.
	*/  
	virtual void OnMusicDone();

	/**
		Set if we have to delete the instance of this class when the music will finish. 
	*/
	void SetAutoDestruct(bool autoDestruct);

	/**
		Get if we have to delete the instance of this class when the music will finish. 
	*/
	bool GetAutoDestruct();

	/**
		Set the volume of the music.
	*/
	void SetVolume(int volume);

	/**
		Returns the current music.
	*/
	KD_Music *GetCurrentMusic();
};

#endif
