#ifndef SurvivalController_H
#define SurvivalController_H

#include "Controller.h"

#include "../game/table.h"
#include "../video/gem.h"
#include "../video/image.h"
#include "../video/imagemanager.h"

class KD_Image;
class KD_Sprite;
class KD_SpriteInstance;
class KD_Font;
class KD_Music;
class KD_Sound;
class KD_TextEvent;

#define NB_LEVELS 15

/**
	The class containing what to do on which action.
  */
class KD_SurvivalController : public KD_Controller
{ protected:
	int score;
	int clashCount;
	int maxClashCount;
	/// The current level of the game!
	int currentLevel;

	/// The number of ticks to wait to add a line.
	int currentTimeBetweenLines;
	/**
		The number of gems that must be dropped to pass each level.
	*/
	int gemsToLevel[NB_LEVELS];

	/**
		The time between each dropping of line per level.
	*/
	int speedOfLevel[NB_LEVELS];

	KD_Table table;

	KD_Sprite *horizontalBar;
	KD_Sprite *verticalBar;
	KD_Sprite *upleftBar;
	KD_Sprite *uprightBar;
	KD_Sprite *leftDoor;
	KD_Sprite *rightDoor;
	KD_Sprite *bottomBar;
	KD_Sprite *clown;
	KD_Sprite *gem[KD_GEM_NB_KINDS];

	KD_Sprite *characterSprite;
	KD_SpriteInstance *characterSpriteInstance;

	KD_TextEvent *comboEvent;

#define KD_SURVIVAL_NB_IMAGES 1
    KD_Image* images[KD_SURVIVAL_NB_IMAGES];
    KD_ImageManager* image_manager;

	KD_Sound *plopSound;

	/**
		The music to be played
	*/
	KD_Music *music;

	/**
		The background image
	*/
	KD_Image *background;

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
	void unLoadSprites();

	void loadMusic(char *fileName);
};

#endif
