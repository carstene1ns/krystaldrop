#ifndef Application_H
#define Application_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "string"
#include "map"

using namespace std;

class KD_Controller;

/**
	Application main class
	It is a singleton.
  */
class KD_Application
{ protected:
	static KD_Application *singleton;

	map<string, KD_Controller *> controllers;
	
	KD_Controller *activeController;
	KD_Controller *askedController;
  
  /* config values (## in a separate class ?) */
    char* config_filename;
    bool have_opengl;
    bool have_sound; /* sound and music */
  /* volume */
    /* and the keyboard */
    

	KD_Application();

	/**
		Test if activeController!=askedController
		If it is, changes the active controller.
		Returns true if the application must quit
		false if we're only asked to change of controller or if we're not asked anything
	*/
	bool processGoto();

public:
	virtual ~KD_Application();

	/**
		Fonction statique retournant le singleton KD_Application.
		Si le singleton n'a pas été instancié, la fonction l'instancie.
	*/
	static KD_Application *getApplication();

	/**
		KD_Application initialisation method
	*/
	virtual bool Init();
    virtual bool GetConfigFileName();
    virtual bool ParseConfigFile();
	virtual bool Loop();

	/**
		KD_Application deinit
	*/
	virtual bool Quit();

	/**
		Method to add a Controller to the application
	*/
	void addController(string name, KD_Controller *controller);

	/**
		Method to remove a Controller to the application
		The controller will be deleted.
	*/
	void removeController(string name);


	/**
		Method to go from a Controller to another
	*/
	void gotoController(KD_Controller *controller);
	void gotoController(string name);

	/**
		Method to get a controller by name
	*/
	KD_Controller *getController(string name);

	
	/**
		Method to stop the application
	*/
	void sendStopEvent();
};

#endif
