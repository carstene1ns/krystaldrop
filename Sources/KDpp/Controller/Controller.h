#ifndef Controller_H
#define Controller_H

#include <SDL/SDL.h>

#include "../Tools/defines.h"
#include "EventManager.h"

/**
	The class containing what to do on which action.
	There should be one controller per screen at least,
	possibly several (for example, several windows)	
  */
#define CONTROLLER_EVENT_KEYDOWN  1
#define CONTROLLER_EVENT_JOYSTICK 2
#define CONTROLLER_EVENT_KEYUP    3
  
class DllExport KD_Controller : public KD_EventManager
{
protected:
	int *keysDownBindings;
	int *keysUpBindings;

public:
	KD_Controller();
	virtual ~KD_Controller();

	/**
		Method to Initialise the Controller.
		It should be used to Process all the key-binding necessary, and even more.
	*/
	virtual bool Init()=0;

	/**
		Method called by the main loop each time an event is received (a key is pressed).
	*/
	virtual bool ProcessEvent(int value)=0;

	/**
		Method called by the main loop each frame to Process real-time stuff.
	*/
	virtual bool Process()=0;

	/**
		Method called by the main loop each time a frame should be Displayed.
	*/
	virtual bool Display()=0;

	/**
		Method called when Quitting the controller.
        Any overriding Quit() function must call KD_Controller::Quit at the end
        Example: return KD_Controller::Quit();
	*/
	virtual bool Quit()=0;

	/**
		Method called when enabling the controller.
	*/
	virtual bool OnEnable();

	/**
		Method called when disabling the controller.
	*/
	virtual bool OnDisable();

	/**
		Method to bind a key to a value that will be sent by ProcessEvent.
		0 stands for NO action.
	*/
	void BindKeyDown(SDLKey key, int value);
	void BindKeyUp(SDLKey key, int value);
  
	/** 
    		Method to bind a key, or a joystick event to a value that will be sent by ProcessEvent.
		# Joystick is not implemented yet !
	        0 stands for no action.
	        Type= CONTROLLER_EVENT_KEYDOWN : event is a SDLKey value
	        Type= CONTROLLER_EVENT_KEYUP   : event is a SDLKey value
	        Type= CONTROLLER_EVENT_JOYSTICK: event is yet to be defined
	*/
	void BindInput(int type, int event, int value);

	//{
	/**
		These methods called by KD_Application take a SDL_Key call the ProcessEvent method, defined by the user.
		0 stands for NO action.
	*/
	virtual bool ProcessKeyDown(SDLKey key);
	virtual bool ProcessKeyUp(SDLKey key);
	//}

	/**
		Method called by KD_Application each time the mouse button is moved.
		The current position as well as the relative move is given.
		xRel and yRel can be used to guess the last position of the mouse.
	*/
	virtual bool ProcessMouseMove(int x, int y, int xRel, int yRel);

	/**
		Method called by KD_Application each time a mouse button is pressed.
		value is the number of the button (1: left, 2: right...) or -1 for a left double click.
	*/
	virtual bool ProcessMouseButtonDown(int button, int x, int y);

	/**
		Method called by KD_Application each time a mouse button is released.
		value is the number of the button (1: left, 2: right...) or -1 for a left double click.
	*/
	virtual bool ProcessMouseButtonUp(int button, int x, int y);
};

#endif
