#ifndef ControlsController_H
#define ControlsController_H

#include "../../KDpp/Controller/WidgetController.h"

#include "../Game/ControlsConfig.h"

class KD_BouncingText;
class KD_Font;
class KD_Music;

/**
	The controller displaying the title screen.
*/
class KD_ControlsController : public KD_WidgetController
{
	/**
		True if a button has been pressed and we are waiting for a key to set.
	*/
	bool isWaitingKeyPress;

	/**
		The key we are waiting for
	*/
	KD_ControlsConfig::KD_Keys keyPressed;

	KD_BouncingText*   pressAnyKeyText;
	KD_Font*           main_font;

#ifndef NO_MUSIC
	KD_Music *music;
#endif

public:
	KD_ControlsController();
	virtual ~KD_ControlsController();

	/**
		Method to initialise the Controller.
		It should be used to process all the key-binding necessary, and even more.
	*/
	virtual bool Init();

	/**
		Method called when quitting the controller.
        Any overriding quit() function must call KD_Controller::quit at the end
        Example: return KD_Controller::quit();
	*/
	virtual bool Quit();

	/**
		Method called by the main loop each time an event is received (a key is pressed).
	*/
	virtual bool ProcessEvent(int value);

	/**
		Method called when enabling the controller.
	*/
	virtual bool OnEnable();

	/**
		Method called when disabling the controller.
	*/
	virtual bool OnDisable();

	void AskForKey();

	string GetButtonNameFromKey(KD_ControlsConfig::KD_Keys key);
};

#endif
