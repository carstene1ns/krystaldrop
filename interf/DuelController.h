#ifndef DuelController_H
#define DuelController_H

#include "Controller.h"

class KD_Hand;
class KD_Parameters;
class KD_Row;
class KD_Gem;
class KD_Sprite;


class KD_DuelController : public KD_Controller
{ 
	KD_Gem *g1, *g2;
	KD_Sprite *spr;
	
/* debug */
KD_Parameters* param;
KD_Hand* hand;
KD_Row* row;
/* /debug */
	

public:
	KD_DuelController();
	virtual ~KD_DuelController();

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
