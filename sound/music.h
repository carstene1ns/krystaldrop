#ifndef Music_H
#define Music_H

#include <SDL_mixer.h>

/**
	A music.
  */
class KD_Music
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
	void setLoop(bool mustLoop);

	signed Load(char *fileName);
	void CloseMusic();

	
	void PlayMusic();
	void StopMusic();

	bool getIsPlaying();

	/**
		Function called by the global callback used in SDL_mixer.
		This function should never be called by the user.
	*/
	void MusicDone();

	/**
		Virtual function not doing anything. It can be overloaded in a subclass to provide some actions when a music finishes.
	*/  
	virtual void onMusicDone();

	/**
		Set if we have to delete the instance of this class when the music will finish. 
	*/
	void setAutoDestruct(bool autoDestruct);

	/**
		Get if we have to delete the instance of this class when the music will finish. 
	*/
	bool getAutoDestruct();

	/**
		Set the volume of the music.
	*/
	void setVolume(int volume);
};

#endif
