#ifndef SurvivalController_H
#define SurvivalController_H

#include "Controller.h"

#include "../game/table.h"

class KD_Sprite;
class KD_SpriteInstance;
class KD_Font;

/**
	The class containing what to do on which action.
  */
class KD_SurvivalController : public KD_Controller
{ 
	KD_Table table;

	KD_Sprite *horizontalBar;
	KD_Sprite *verticalBar;
	KD_Sprite *upleftBar;
	KD_Sprite *uprightBar;

	KD_Sprite *clown;

public:
	KD_SurvivalController();
	virtual ~KD_SurvivalController();

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

	void loadSprites();
};

#endif
