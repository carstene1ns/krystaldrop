#include "../global.h"

#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "Application.h"
#include "CharSelectController.h"
#include "CharSelect2Controller.h"
#include "DuelController.h"
#include "eventmanager.h"
#include "HighScoresController.h"
#include "keyboard.h"
#include "MenuController.h"
#include "SurvivalController.h"
#include "TitleController.h"

#include "../config.h"
#include "../util/logfile.h"
#ifndef NO_SOUND
#include "../sound/soundsystem.h"
#endif
#include "../video/background.h"
#include "../video/Display.h"
#include "../video/imagemanager.h"


/// unique application
KD_Application *KD_Application::singleton=0;

KD_Application::KD_Application()
{
	askedController=0;
	activeController=0;
  	config= NULL;
}

KD_Application::~KD_Application()
{
  if (config!= NULL)
  { /* ## free config ? ## */
    config= NULL;
  }
  
  singleton= NULL;
}

KD_Application *KD_Application::getApplication()
{
	if (singleton== 0) singleton= new KD_Application();
	return singleton;
}

bool KD_Application::Init()
{
	srand ( (unsigned) time(NULL) );
  
#ifdef DEBUG
	KD_LogFile::Init("kd_log.txt");
#endif  

	// Does not initiate anything (initialisation will be done later...)
    if ( SDL_Init(0) < 0 ) {
        fprintf(stderr, "SDL could not be initialised : %s\n", SDL_GetError());
        return false;
    }

    if (ParseConfigFile()== false)
      fprintf (stderr, "Could not parse configuration file, using default values\n");

    assert (config);
    Display::initDisplay(SCR_W, SCR_H, 32, !(config->fullscreen), config->opengl);

#ifndef NO_SOUND    
	KD_SoundSystem::initSoundSystem(22050, 16, true);
#endif    

	KD_Keyboard::initKeyboard();
	KD_EventManager::initEventManager();

    addController("title", new KD_TitleController());
    addController("menu", new KD_MenuController());
    addController("charsel", new KD_CharSelectController()); /* after Title ! */    
    addController("charsel2", new KD_CharSelect2Controller()); /* after Title ! */        
    addController("highscores", new KD_HighScoresController());
	addController("survival", new KD_SurvivalController());
	addController("duel", new KD_DuelController());
	gotoController ("title");
	
	return true;
}


bool KD_Application::ParseConfigFile()
{ if (config== NULL) config= KD_Config::GetConfig();
  assert (config);
    
  if (config->FindConfigFile()== false) 
  { fprintf (stderr, "Could not find configuration file. Default values used.\n");
    return true; 
  }
  
  return config->ReadConfig();
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

		KD_Keyboard::getKeyboard()->resetLastKey();

		SDL_Event event;

		if (processGoto()) break;
		
		/* Check for events */
		/* Loop until there are no events left on the queue */
		while(SDL_PollEvent(&event))
		{
			if (processGoto()) break;
			

			switch(event.type)
			{  
				/* Process the appropriate event type */
				case SDL_QUIT:
					sendStopEvent();
					break;
				case SDL_KEYDOWN:	/* Handle a KEYDOWN event */
					KD_Keyboard::getKeyboard()->setLastKey(event);
					activeController->processKeyDown(event.key.keysym.sym);
					break;
				case SDL_KEYUP:		/* Handle a KEYUP event */
					activeController->processKeyUp(event.key.keysym.sym);
					break;
			}
		}

		if (askedController == 0)
			break;
		
		KD_EventManager::getEventManager()->UpdateEvents();

		activeController->display();

		KD_EventManager::getEventManager()->DisplayEvents();

		Display::flip();

	} while (/*askedController!=0*/1);

	if (activeController != 0)
		activeController->quit();

	return true;
}

bool KD_Application::processGoto()
{
	/**
		Processing of the goto statement or of the end of application.
	*/
	if (askedController != activeController)
	{
      /* if there a timer running ? */
      if (timer> 0) 
       if (SDL_GetTicks()- last_time < timer) /* timer on, and still running */
        return false;
   
      timer= 0;    

      // If there was a previous controller (if we are not starting the application)
		if (activeController != 0)
			activeController->quit();
		
		activeController = askedController;
		
		// process the application end event!
		if (askedController == 0)
			return true;
		
		activeController->init();
	}
	return false;
}

bool KD_Application::Quit()
{
	KD_EventManager::closeEventManager();

	removeController("duel");
	removeController("survival");  
    removeController("highscores");
	removeController("charsel");
	removeController("charsel2");  
    removeController("menu");
    removeController("title");

	KD_Keyboard::closeKeyboard();
  
    KD_Background* back= KD_Background::GetBackground();
    if (back!= NULL) delete back;

	delete KD_ImageManager::getImageManager();

#ifndef NO_SOUND    
	KD_SoundSystem::deInit();
#endif    
	Display::deInit();

#ifdef DEBUG    
	KD_LogFile::Close();
#endif
    
	return true;
}

void KD_Application::addController(string name, KD_Controller *controller)
{
	controllers[name]=controller;
}

void KD_Application::removeController(string name)
{
	KD_Controller *cont = controllers[name];

/*	short size= controllers.size();
	short i;

	for (i= 0; i< size; i++)
		if (controllers[i] == cont)
			break;

	assert(i==size);

	controllers.erase(controllers.begin()+ i);*/

/*	map<string, KD_Controller *>::iterator it = controllers.begin();

	printf("Removing controller %s\n",name.c_str);
	while (it != controllers.end())
	{
		string contr = (*it).first;
		printf("%s\n",contr.c_str()) ;
		it++;
	}
*/

#ifndef NDEBUG
	int nb_removed =
#endif 
    controllers.erase(name);
	assert (nb_removed== 1);

	delete cont;

}

void KD_Application::gotoController(KD_Controller *controller)
{
  timer= 0; 
  askedController= controller;
}

void KD_Application::gotoController(string name)
{
  gotoController(controllers[name]);
}

void KD_Application::DelayedGotoController (KD_Controller *controller, Uint32 when)
{ 
  last_time= SDL_GetTicks();
  timer= when;
  askedController= controller;  
}

void KD_Application::DelayedGotoController (string name, Uint32 when)
{
  DelayedGotoController (controllers[name], when);
}


KD_Controller *KD_Application::getController(string name)
{
	return controllers[name];
}

void KD_Application::sendStopEvent()
{
	askedController = 0;
}
