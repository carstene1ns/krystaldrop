#include <assert.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "Application.h"
//#include "eventmanager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/GlobalResourceSet.h"
#include "UserInterface/Keyboard.h"
#include "UserInterface/Mouse.h"
#include "Controller.h"
#include "../Tools/XMLConfig.h"
#include "../Tools/Logfile.h"

#include "../Video/Display.h"
#include "../Sound/SoundSystem.h"

/// unique application
KD_Application *KD_Application::singleton= NULL;

KD_Application::KD_Application()
{
	stopEvent = false;
	config = 0;
}

KD_Application::~KD_Application()
{
	singleton = NULL;
}

KD_Application *KD_Application::GetApplication()
{
	if (singleton== NULL) singleton= new KD_Application();
	return singleton;
}


bool KD_Application::InitFromConfigFile()
{
	// Check that no configuration file has ever been loaded
	assert(!config);

	config = new KD_XMLConfig();
	bool res = config->Load();
	
	if (!res) return false;

	return InitFromConfigObject(config);
}
/*
bool KD_Application::InitFromConfigFile(const string &configFile)
{
	// Check that no configuration file has ever been loaded
	assert(!config);

	config = new KD_XMLConfig();
	bool res = config->Load(configFile);
    if (!res) return false;

	return InitFromConfigObject(config);
}*/

bool KD_Application::InitFromConfigObject(KD_XMLConfig *config)
{
	bool res;

	res = Init();
	if (!res) return false;
	
	KD_ResourceManager::InitResourceManager();
	KD_GlobalResourceSet::InitGlobalResourceSet();
	
	bool fullscreen = config->GetFullScreen();
	bool openGL = config->GetOpenGL();
	res = InitVideoSystem(640,480,32,fullscreen,openGL);
	if (!res) return false;

	res = InitIOSystem();
	if (!res) return false;

	bool sound = config->GetEnableSound();

	if (sound)
	{
		int freq = config->GetSoundFrequency();
		int bits = config->GetSoundBits();
		bool stereo = config->GetStereoSound();

		if (!InitSoundSystem(freq,bits,stereo) ) return false;
	}

	// INCLURE LE NO SOUND DANS LE CODE!

	return true;
}

bool KD_Application::Init()
{
	srand ( (unsigned) time(NULL) );

	// Does not Initiate anything (Initialisation will be done later...)
	if ( SDL_Init(0) < 0 ) {
		fprintf(stderr, "SDL could not be Initialised : %s\n", SDL_GetError());
		return false;
    }
	
	return true;
}

bool KD_Application::InitVideoSystem(int width, int height, int bits, bool fullscreen, bool openGL)
{
	Display::InitDisplay(width, height, bits, !(fullscreen), openGL);
	return true;
}

bool KD_Application::InitIOSystem()
{
	KD_Keyboard::InitKeyboard();
	KD_Mouse::InitMouse();
	return true;
}

bool KD_Application::InitSoundSystem(int freq, int bits, bool stereo)
{
	KD_SoundSystem::InitSoundSystem(freq, bits, stereo);
	return true;
}

bool KD_Application::CloseVideoSystem()
{
	Display::deInit();
	return true;
}

bool KD_Application::CloseSoundSystem()
{
	KD_SoundSystem::deInit();
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

		KD_Keyboard::GetKeyboard()->ReSetLastKey();

		SDL_Event event;
		
		/* Check for events */
		/* Loop until there are no events left on the queue */
		while(SDL_PollEvent(&event))
		{


			switch(event.type)
			{  
				/* Process the appropriate event type */
				case SDL_QUIT:
					SendStopEvent();
					break;
				case SDL_KEYDOWN:	/* Handle a KEYDOWN event */
					KD_Keyboard::GetKeyboard()->SetLastKey(event);
					for (unsigned int i=0; i<activeControllers.size(); i++)
					{
						bool res = activeControllers[i]->ProcessKeyDown(event.key.keysym.sym);
						// If the event has been handled, don't send it to the other controllers.
						if (res) break;
					}
					break;
				case SDL_KEYUP:		/* Handle a KEYUP event */
					for (unsigned int i=0; i<activeControllers.size(); i++)
					{
						bool res = activeControllers[i]->ProcessKeyUp(event.key.keysym.sym);
						// If the event has been handled, don't send it to the other controllers.
						if (res) break;
					}
					break;
				case SDL_MOUSEMOTION:
					KD_Mouse::GetMouse()->UpdateMousePosition(event);
					for (unsigned int i=0; i<activeControllers.size(); i++)
						{
							bool res = activeControllers[i]->ProcessMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
							// If the event has been handled, don't send it to the other controllers.
							if (res) break;
						}
					break;
				case SDL_MOUSEBUTTONDOWN:
					{
						int button = KD_Mouse::GetMouse()->onButtonDown(event);
						for (unsigned int i=0; i<activeControllers.size(); i++)
						{
							bool res = activeControllers[i]->ProcessMouseButtonDown(button, event.button.x, event.button.y);
							// If the event has been handled, don't send it to the other controllers.
							if (res) break;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					{
						int button = KD_Mouse::GetMouse()->onButtonUp(event);
						for (unsigned int i=0; i<activeControllers.size(); i++)
						{
							bool res = activeControllers[i]->ProcessMouseButtonUp(button, event.button.x, event.button.y);
							// If the event has been handled, don't send it to the other controllers.
							if (res) break;
						}
					}
					break;
			}
		}

		for (unsigned int i=0; i<activeControllers.size(); i++)
		{
			activeControllers[i]->Process();
			activeControllers[i]->UpdateEvents();
		}

		// Display in reverse order
		for (int i=(int)activeControllers.size()-1; i>=0; i--)
		{
			activeControllers[i]->Display();
			activeControllers[i]->DisplayEvents();
		}

		// First, let's remove controllers
		while(!toRemoveActiveControllers.empty())
		{
			// for each controller to remove, first, let's try to remove it from the activeControllers list.
			bool found=false;
			for (unsigned int i=0; i<activeControllers.size(); i++)
			{
				if (toRemoveActiveControllers[0] == activeControllers[i])
				{
					found = true;
					activeControllers[i]->OnDisable();
					activeControllers[i]->DeleteAllEvents();
					activeControllers.erase(activeControllers.begin()+i);
					toRemoveActiveControllers.pop_front();
					break;
				}
			}
			// if the controller was not found in the active list, maybe it is not yet added... let's scan the toAddActiveController list...
			if (found == false)
			{
				for (unsigned int i=0; i<toAddActiveControllers.size(); i++)
				{
					if (toRemoveActiveControllers[0] == toAddActiveControllers[i])
					{
						found = true;
						// Enable and disable it right after!
						toAddActiveControllers[i]->OnEnable();
						toAddActiveControllers[i]->OnDisable();
						toAddActiveControllers[i]->DeleteAllEvents();
						toAddActiveControllers.erase(toAddActiveControllers.begin()+i);
						toAddActiveControllersPosition.erase(toAddActiveControllersPosition.begin()+i);
						toRemoveActiveControllers.pop_front();
						break;
					}
				}
				// if the controller to be removed is not found, then there is a serious problem.
				assert(found);
			}
		}

		// Now, let's add some new controllers...
		while(!toAddActiveControllers.empty())
		{
			int pos = toAddActiveControllersPosition[toAddActiveControllersPosition.size()-1];
			KD_Controller *cont = toAddActiveControllers[toAddActiveControllers.size()-1];
			if (pos == KD_LAST_POS)
				activeControllers.push_back(cont);
			else
                activeControllers.insert(activeControllers.begin()+pos, cont);

			toAddActiveControllers.pop_back();
			toAddActiveControllersPosition.pop_back();
			cont->OnEnable();
		}

		Display::flip();

		// Invariant: here, toAddActiveControllers and toRemoveActiveControllers should be empty.
		assert(toAddActiveControllers.size() == 0);
		assert(toAddActiveControllersPosition.size() == 0);
		assert(toRemoveActiveControllers.size() == 0);

	} while (!stopEvent && activeControllers.size()!=0);

	while(!activeControllers.empty())
	{
		activeControllers[0]->OnDisable();
		activeControllers[0]->DeleteAllEvents();
		activeControllers.pop_front();
	}

	return true;
}


bool KD_Application::Quit()
{
	if (!CloseSoundSystem() ) return false;
	if (!CloseVideoSystem() ) return false;
	KD_GlobalResourceSet::CloseGlobalResourceSet();
	KD_ResourceManager::CloseResourceManager();
	if (!Quit()) return false;

	if (!config->Unload()) return false;
	delete config;
	config = 0;

	return true;
}

void KD_Application::RegisterController(string name, KD_Controller *controller)
{
	controllers[name]=controller;
	controller->Init();
}

void KD_Application::UnregisterController(string name)
{
	KD_Controller *cont = controllers[name];
	cont->Quit();

	controllers.erase(name);

	delete cont;
}

bool KD_Application::isActive(string name)
{
	return isActive(controllers[name]);
}

bool KD_Application::isActive(KD_Controller *controller) const
{
	for (unsigned int i=0; i<activeControllers.size(); i++)
		if (activeControllers[i] == controller)
			return true;

	for (unsigned int i=0; i<toAddActiveControllers.size(); i++)
		if (toAddActiveControllers[i] == controller)
			return true;

	return false;
}

void KD_Application::EnableController(string name, int position)
{
	// first, in debug release, test if we are not trying to add a controller that is already active...
	assert(!isActive(name));

	toAddActiveControllers.push_back(GetController(name));
	toAddActiveControllersPosition.push_back(position);
}

void KD_Application::MoveControllerToPos(string name, int position)
{
	assert(isActive(name));

	if (position>=(int)activeControllers.size())
		position = (int)activeControllers.size()-1;

	KD_Controller *cont = controllers[name];

	unsigned int i;
    for (i=0; i<activeControllers.size(); i++)
		if (cont == activeControllers[i])
		{
			activeControllers.erase(activeControllers.begin()+i);
			activeControllers.insert(activeControllers.begin()+position, cont);
			break;
		}

	// if failed, the active controller must be in the toAdd list.
	if (i == activeControllers.size())
		for (i=0; i<toAddActiveControllers.size(); i++)
			if (cont == toAddActiveControllers[i])
			{
				toAddActiveControllersPosition[i] = position;
				break;
			}

}

void KD_Application::DisableController(string name)
{
	// first, in debug release, check if the controller to be disabled is active.
	assert(isActive(name));

	toRemoveActiveControllers.push_back(GetController(name));
}

void KD_Application::DisableController(KD_Controller *controller)
{
	toRemoveActiveControllers.push_back(controller);
}

KD_Controller *KD_Application::GetController(string name)
{
	return controllers[name];
}

void KD_Application::SendStopEvent()
{
	stopEvent=true;
}

KD_XMLConfig *KD_Application::GetConfigFile()
{
	return config;
}
