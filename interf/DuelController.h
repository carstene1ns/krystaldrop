#ifndef DuelController_H
#define DuelController_H

#include "Controller.h"

class KD_Parameters;
class KD_Gem;
class KD_Set;
class KD_Sprite;
class KD_SpriteInstance;
class KD_Font;


class KD_DuelController : public KD_Controller
{ 
  KD_Gem *g1, *g2;
  KD_Sprite *spr, *blue_spr;
  KD_Font *smallFont;
  KD_SpriteInstance *sprInst;	
  KD_Parameters* param;  
  KD_Set* set;

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
