#ifndef Controller_H
#define Controller_H

#include "SDL/SDL.h"

/*#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "map"

using namespace std;
*/

/**
	The class containing what to do on which action.
	There should be one controller per screen
	(eg one for the starting screen, one for the option screen, one for the game, etc....)
  */
class KD_Controller
{ 
	int *keysDownBindings;
	int *keysUpBindings;

//	map<SDLKey, int> keysDownBindings;
//	map<SDLKey, int> keysUpBindings;

public:
	KD_Controller();
	virtual ~KD_Controller();

	/**
		Method to initialise the Controller.
		It should be used to process all the key-binding necessary, and even more.
	*/
	virtual bool init()=0;

	/**
		Method called by the main loop each time an event is received (a key is pressed).
	*/
	virtual bool processEvent(int value)=0;

	/**
		Method called by the main loop each time a frame should be displayed.
	*/
	virtual bool display()=0;

	/**
		Method called when quitting the controller.
	*/
	virtual bool quit()=0;

	/**
		Method to bind a key to a value that will be send by processEvent.
		0 stands for NO action.
	*/
	void bindKeyDown(SDLKey key, int value);
	void bindKeyUp(SDLKey key, int value);

	/**
		These methods called by KD_Application take a SDL_Key call the processEvent method, defined by the user.
		0 stands for NO action.
	*/
	void processKeyDown(SDLKey key);
	void processKeyUp(SDLKey key);
};

#endif
