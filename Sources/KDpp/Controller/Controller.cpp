#include "Controller.h"

KD_Controller::KD_Controller() : KD_EventManager()
{
	int i;
  
	keysDownBindings = new int[SDLK_LAST]; 
	for (i=0; i<SDLK_LAST; i++) keysDownBindings[i]=0;

	keysUpBindings = new int[SDLK_LAST]; 
	for (i=0; i<SDLK_LAST; i++) keysUpBindings[i]=0;
    
}

KD_Controller::~KD_Controller()
{
	delete[] keysDownBindings;
	delete[] keysUpBindings;
}

bool KD_Controller::OnEnable()
{
	return false;
}

bool KD_Controller::OnDisable()
{
	return false;
}

void KD_Controller::BindInput(int type, int event, int value)
{
  switch (type)
  { case CONTROLLER_EVENT_KEYDOWN:
      BindKeyDown ((SDLKey) event, value);
      return;
    case CONTROLLER_EVENT_KEYUP:
      BindKeyUp ((SDLKey) event, value);
      return;
    case CONTROLLER_EVENT_JOYSTICK: 
      /* not implemented */
      return;
    default:
      /* an assert could be useful here */
      return;
  }
}

void KD_Controller::BindKeyDown(SDLKey key, int value)
{
	keysDownBindings[key] = value;
}

void KD_Controller::BindKeyUp(SDLKey key, int value)
{
	keysUpBindings[key] = value;
}

bool KD_Controller::ProcessKeyDown(SDLKey key)
{
	return ProcessEvent(keysDownBindings[key]);
}

bool KD_Controller::ProcessKeyUp(SDLKey key)
{
	return ProcessEvent(keysUpBindings[key]);
}

bool KD_Controller::ProcessMouseMove(int x, int y, int xRel, int yRel)
{
	return false;
}

bool KD_Controller::ProcessMouseButtonDown(int button, int x, int y)
{
	return false;
}

bool KD_Controller::ProcessMouseButtonUp(int button, int x, int y)
{
	return false;
}
