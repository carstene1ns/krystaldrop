#include "Application.h"

#include <stdlib.h>
#include "SDL/SDL.h"

#include "../video/Display.h"
#include "../video/imagemanager.h"
#include "../util/logfile.h"

#include "DuelController.h"
#include "StartController.h"

/// Singleton représentant l'Application
KD_Application *KD_Application::singleton=0;

KD_Application::KD_Application()
{
	askedController=0;
	activeController=0;
}

KD_Application::~KD_Application()
{

}

KD_Application *KD_Application::getApplication()
{
	if (singleton==0) singleton = new KD_Application();
	return singleton;
}

bool KD_Application::Init()
{
	KD_LogFile::Init("log.txt");

	// Does not initiate anything (initialisation will be done later...)
    if ( SDL_Init(0) < 0 ) {
        fprintf(stderr, "SDL could not be initialised : %s\n", SDL_GetError());
        return false;
    }

	Display::initDisplay(800,600,32,true,false);



	addController("start", new KD_StartController());
	addController("duel", new KD_DuelController());
	gotoController("start");
//    gotoController ("duel");
	
	
	return true;
}

/**
	KD_Application main loop
*/
bool KD_Application::Loop()
{


	do
	{
		// Check if socket data available
		// Perhaps send data over the network
		// Draw
		// SDL_Sound

		SDL_Event event;
		
		/* Check for events */
		/* Loop until there are no events left on the queue */
		while(SDL_PollEvent(&event))
		{
			/**
				Processing of the goto statement or of the end of application.
			*/
			if (askedController != activeController)
			{
				// If there was a previous controller (if we are not starting the application)
				if (activeController != 0)
					activeController->quit();
				
				activeController = askedController;
				
				// process the application end event!
				if (askedController == 0)
					break;
				
				activeController->init();
			}

			switch(event.type)
			{  
				/* Process the appropiate event type */
				case SDL_QUIT:
					sendStopEvent();
					break;
				case SDL_KEYDOWN:	/* Handle a KEYDOWN event */         
					activeController->processKeyDown(event.key.keysym.sym);
					break;
				case SDL_KEYUP:		/* Handle a KEYUP event */
					activeController->processKeyUp(event.key.keysym.sym);
					break;
			}

			
		}

		activeController->display();
		Display::flip();

	} while (askedController!=0);

	if (activeController != 0)
		activeController->quit();

	return true;
}

bool KD_Application::Quit()
{
	
	removeController("start");
	removeController("duel");

	delete KD_ImageManager::getImageManager();

	Display::deInit();

	KD_LogFile::Close();
	return true;
}

void KD_Application::addController(string name, KD_Controller *controller)
{
	controllers[name]=controller;
}

void KD_Application::removeController(string name)
{
	KD_Controller *cont = controllers[name];
	controllers.erase(name);
	delete cont;
}

void KD_Application::gotoController(KD_Controller *controller)
{
	askedController = controller;
}

void KD_Application::gotoController(string name)
{
	gotoController(controllers[name]);
}

KD_Controller *KD_Application::getController(string name)
{
	return controllers[name];
}

void KD_Application::sendStopEvent()
{
	askedController = 0;
}
