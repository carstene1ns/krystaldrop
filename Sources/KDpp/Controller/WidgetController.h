#ifndef WidGetController_H
#define WidGetController_H

#include <SDL/SDL.h>

#include "../Tools/defines.h"

#include "Controller.h"
#include "../Resources/ResourceSet.h"
#include "UserInterface/WidgetContainer.h"

/**
	A Controller implemeting a WidgetContainer (and a ResourceSet)
	The controller forwards all the events to the widgets before forwarding them to the usual event system.
	This kind of controller should be used each time a widget is used.
  */
class DllExport KD_WidgetController : public KD_Controller, public KD_ResourceSet, public KD_WidgetContainer
{
protected:
	

public:
	KD_WidgetController();
	virtual ~KD_WidgetController();

	/**
		Method to Initialise the Controller.
		It should be used to Process all the key-binding necessary, and even more.
	*/
	virtual bool Init()=0;

	/**
		Method called by the main loop each time an event is received (a key is pressed).
	*/
	virtual bool ProcessEvent(int value);

	/**
		Method called by the main loop each frame to Process real-time stuff.
	*/
	virtual bool Process();

	/**
		Method called by the main loop each time a frame should be Displayed.
	*/
	virtual bool Display();

	/**
		Method called when quitting the controller.
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
