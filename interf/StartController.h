#ifndef StartController_H
#define StartController_H

#include "Controller.h"

/**
	The class containing what to do on which action.
  */
class KD_StartController : public KD_Controller
{ 
public:
	KD_StartController();
	virtual ~KD_StartController();

	/**
		Method to initialise the Controller.
		It should be used to process all the key-binding necessary, and even more.
	*/
	virtual bool init();

	/**
		Method called by the main loop each time an event is received (a key is pressed).
	*/
	virtual bool processEvent(int value);

	/**
		Method called by the main loop each time a frame should be displayed.
	*/
	virtual bool display();

	/**
		Method called when quitting the controller.
	*/
	virtual bool quit();
};

#endif
