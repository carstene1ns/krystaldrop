#include "../global.h"

#include "Controller.h"
#ifndef NO_MUSIC
#include "../sound/music.h"
#endif 

KD_Controller::KD_Controller()
{	int i;
  
	keysDownBindings = new int[SDLK_LAST]; 
	for (i=0; i<SDLK_LAST; i++) keysDownBindings[i]=0;

	keysUpBindings = new int[SDLK_LAST]; 
	for (i=0; i<SDLK_LAST; i++) keysUpBindings[i]=0;
    
#ifndef NO_MUSIC    
    music= new KD_Music;
    assert (music);
#endif    
}

KD_Controller::~KD_Controller()
{
	delete[] keysDownBindings;
	delete[] keysUpBindings;
#ifndef NO_MUSIC  
	DELETE (music);
#endif  
}

bool KD_Controller::quit()
{ KD_EventManager::getEventManager()->deleteAllEvents();
/* if we leave some events running when changing controllers, 
   the program will crash */
  return true;
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
