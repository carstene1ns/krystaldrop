#include "StartController.h"

#include "Application.h"

KD_StartController::KD_StartController() : KD_Controller()
{

}

KD_StartController::~KD_StartController()
{

}


bool KD_StartController::init()
{
	// Warning! Never use action 0!!!!!
	bindKeyDown(SDLK_ESCAPE, 1);
	bindKeyDown(SDLK_UP, 2);
	bindKeyDown(SDLK_DOWN, 3);
	

	return true;
}

bool KD_StartController::processEvent(int value)
{
	switch(value)
	{
		case 1:
			KD_Application::getApplication()->sendStopEvent();
			return true;
		case 2:
			printf("up keydown!\n");
			return true;
		case 3:
			printf("down keydown!\n");
			return true;
	}

	return false;
}

bool KD_StartController::display()
{

	return true;
}

bool KD_StartController::quit()
{

	return true;
}
