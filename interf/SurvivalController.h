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

#ifndef NO_MUSIC
class KD_Music;
#endif

#ifndef NO_SOUND
class KD_Sound;
#endif

class KD_InputBox;
class KD_TextEvent;

#define NB_LEVELS 15

#define KD_CSTATE_READY 0
#define KD_CSTATE_PLAYING 1
#define KD_CSTATE_LOSE 2
#define KD_CSTATE_WIN 3
#define KD_CSTATE_HIGHSCORE 4

/**
	The class containing what to do on which action.
  */
class KD_SurvivalController : public KD_Controller
{ protected:
	/**
		The state of the controller.
		It is one of the values defines by KD_CSTATE_.....
		KD_CSTATE_READY is when the controller is printing "Ready? Go!"
		KD_CSTATE_PLAYING : the controller is in its normal state (the party is going on)
		KD_CSTATE_LOSE: The player lost. The balls are all dropping from the screen
		KD_CSTATE_WIN: The player won. The balls are disappearing at top of screen
		KD_CSTATE_HIGHSCORE: THe player is entering its name for the high scores
	*/
	short controllerState;

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

	KD_Sprite *particle;

	KD_TextEvent *comboEvent;
	KD_TextEvent *timer;

	KD_InputBox *nameBox;

	/**
		The time at which the player lost or won.
		Used to change controller after a while.
	*/
	int timeOfNewState;

#define KD_SURVIVAL_NB_IMAGES 1
    KD_Image* images[KD_SURVIVAL_NB_IMAGES];
    KD_ImageManager* image_manager;

#ifndef NO_SOUND
	KD_Sound *plopSound;
#endif    

	/**
		The music to be played
	*/
    #ifndef NO_MUSIC
	KD_Music *music;
    #endif

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
		The method called to display the screen and update the table when we are in KD_CSTATE_PLAYING mode.
	*/
	bool displayPlayingState();

	/**
		The method called to display the screen and update the table when we are in KD_CSTATE_LOSE mode.
	*/
	bool displayLoseState();

	/**
		The method called to display the screen and update the table when we are in KD_CSTATE_HIGHSCORE mode.
	*/
	bool displayHighScoreState();


	/**
		Method called when quitting the controller.
	*/
	virtual bool quit();

	void loadSprites();
	void unLoadSprites();

#ifndef NO_MUSIC
	void loadMusic(char *fileName);
#endif
};

#endif
