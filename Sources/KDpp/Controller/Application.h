#ifndef Application_H
#define Application_H

#include "../Tools/defines.h"

#include <string>
#include <deque>
#include <map>
#include "SDL/SDL.h"

#include "../Tools/FilePath.h"

using namespace std;

/// Position of the controllers added to the active list.
#define KD_FRONT_POS 0
#define KD_LAST_POS -1

class KD_Controller;
class KD_XMLConfig;

/**
	Application main class
	It is a singleton.
  */
class DllExport KD_Application
{ private:
	/// Pointer to the configuration file
	KD_XMLConfig *config;

	map<string, KD_Controller *> controllers;

	/// deque of controllers that are active.
	deque<KD_Controller *> activeControllers;
	/// deque of controllers that will be added to the list of active controllers next frame.
	deque<KD_Controller *> toAddActiveControllers;
	/// deque of positions where the controllers to be added should be put in.
	deque<int> toAddActiveControllersPosition;
	/// deque of controllers to be removed.
	deque<KD_Controller *> toRemoveActiveControllers;
	
	/// true when this is the last frame.
	bool stopEvent;
  
protected:
	virtual bool InitFromConfigObject(KD_XMLConfig *config);
	static KD_Application *singleton;
	static KD_Application *getInternalApplication();
	static void setInternalApplication(KD_Application *application);
	KD_Application();

public:
	virtual ~KD_Application();

	/**
		Fonction statique retournant le singleton KD_Application.
		Si le singleton n'a pas été instancié, la fonction l'instancie.
	*/
	static KD_Application *GetApplication();

	/**
		KD_Application Initialisation method
	*/
	virtual bool InitFromConfigFile();
	//virtual bool InitFromConfigFile(const string &configFile);
public:
	virtual bool Init();
	virtual bool InitVideoSystem(int width, int height, int bits, bool fullscreen, bool openGL);

	/// IOSystem must be Initialised after video system.
	virtual bool InitIOSystem();
#ifndef NO_MIXER
	virtual bool InitSoundSystem(int freq, int bits, bool stereo);
#endif
	virtual bool CloseVideoSystem();
#ifndef NO_MIXER
	virtual bool CloseSoundSystem();
#endif
	virtual bool Loop();

	/**
		KD_Application deInit
	*/
	virtual bool Quit();

	/**
		Method to add a Controller to the application (not in the active list)
		As soon as a controller is registered, the Init moethod of the controller is called.
	*/
	void RegisterController(string name, KD_Controller *controller);

	/**
		Method to remove a Controller from the application (not from the active list)
		The Quit method of the controller will be called, then the controller will be deleted.
	*/
	void UnregisterController(string name);

	//{
	/**
		Returns true if the given controller is active (or going to be active on next frame), false otherwise.
		This method is Quite slow, so it should be used sparsly.
	*/
	bool isActive(string name);
	bool isActive(KD_Controller *controller) const;
	//}

	/**
		Enable a controller previously registered (puts the controller in the active list).
		The position can be specified. If not, it will be put at the front position.
		2 special values exist: KD_FRONT_POS -> front position, KD_LAST_POS -> last position
		If 2 controllers ask for the same position, the last one asking for the position wins.
	*/
	void EnableController(string name, int position=KD_FRONT_POS);

	/**
		Moves an active controller at the specified position in the list of controllers.
	*/
	void MoveControllerToPos(string name, int position);

	//{
	/**
		Disable an active controller (this removes the controller from the active list).
	*/
	void DisableController(string name);
	void DisableController(KD_Controller *controller);
	//}

	/**
		Method to get a controller by name
	*/
	KD_Controller *GetController(string name);

	/**
		Method to stop the application
	*/
	void SendStopEvent();

	/**
		Returns the config file.
	*/
	KD_XMLConfig *GetConfigFile();
};

#endif
