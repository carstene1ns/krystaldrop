#include "Controller.h"

KD_Controller::KD_Controller()
{
	int i;
	keysDownBindings = new int[SDLK_LAST]; 
	for (i=0; i<SDLK_LAST; i++)
		keysDownBindings[i]=0;

	keysUpBindings = new int[SDLK_LAST]; 
	for (i=0; i<SDLK_LAST; i++)
		keysUpBindings[i]=0;
}

KD_Controller::~KD_Controller()
{
	delete[] keysDownBindings;
	delete[] keysUpBindings;
}

void KD_Controller::bindKeyDown(SDLKey key, int value)
{
	keysDownBindings[key] = value;
}

void KD_Controller::bindKeyUp(SDLKey key, int value)
{
	keysUpBindings[key] = value;
}

void KD_Controller::processKeyDown(SDLKey key)
{
	processEvent(keysDownBindings[key]);
}

void KD_Controller::processKeyUp(SDLKey key)
{
	processEvent(keysUpBindings[key]);
}
