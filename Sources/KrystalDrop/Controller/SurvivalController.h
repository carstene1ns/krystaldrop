#ifndef SurvivalController_H
#define SurvivalController_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"

#include "../Game/Table.h"
#include "../Video/Gem.h"
#include "../../KDpp/Video/Image.h"
//#include "../Video/imagemanager.h"

class KD_Font;
class KD_Image;
class KD_InputBox;
class KD_Sound;
class KD_Sprite;
class KD_SpriteInstance;
class KD_TextEvent;
class KD_Music;

#define NB_LEVELS 15

#define KD_CSTATE_READY 0
#define KD_CSTATE_PLAYING 1
#define KD_CSTATE_LOSE 2
#define KD_CSTATE_WIN 3
#define KD_CSTATE_HIGHSCORE 4

/**
	The class containing what to do on which action.
  */
class KD_SurvivalController : public KD_Controller, KD_ResourceSet
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

	#define KD_GOODMOOD 0
	#define KD_MEDIUMMOOD 1
	#define KD_STRESSED 2
	/**
		Current mood of the character:
			0 = good mood
			1 = medium mood (1/2 of screen full)
			2 = stressed (3/4 of screen full)
			CURRENTLY, ONLY 2 IS IMPLEMENTED
	*/
	int characterMood;

	KD_Table table;

	KD_Sprite *horizontalBar;
	KD_Sprite *verticalBar;
	KD_Sprite *upleftBar;
	KD_Sprite *uprightBar;
	KD_Sprite *leftDoor;
	KD_Sprite *rightDoor;
	KD_Sprite *bottomBar;
	KD_Sprite *lineSprite;
	//KD_Sprite *clown;
	KD_Sprite *gem[KD_GEM_NB_KINDS];

	//KD_Sprite *characterSprite;
	//KD_SpriteInstance *characterSpriteInstance;

	KD_Sprite *particle;

	KD_TextEvent *comboEvent;
	KD_TextEvent *timer;

	KD_InputBox *nameBox;

	KD_Font* main_font;

	/**
		The time at which the player lost or won.
		Used to change controller after a while.
	*/
	int timeOfNewState;

#define KD_SURVIVAL_NB_IMAGES 1
    KD_Image* images[KD_SURVIVAL_NB_IMAGES];

#ifndef NO_SOUND
	KD_Sound *plopSound;
	KD_Sound *gemsDownSound;
	KD_Sound *gemsUpSound;
	KD_Sound *chocSound;
	KD_Sound *clashSound[KD_SND_NBCLASHSOUND];
#endif

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
		Method to Initialise the Controller.
		It should be used to Process all the key-binding necessary, and even more.
	*/
	virtual bool Init();

	/**
		Method called by the main loop each time an event is received (a key is pressed).
	*/
	virtual bool ProcessEvent(int value);

	virtual bool Process();

	/**
		Method called by the main loop each time a frame should be Displayed.
	*/
	virtual bool Display();


	/**
		The method called to Display the screen and update the table when we are in KD_CSTATE_PLAYING mode.
	*/
	bool DisplayPlayingState();

	/**
		The method called to Display the screen and update the table when we are in KD_CSTATE_LOSE mode.
	*/
	bool DisplayLoseState();

	/**
		The method called to Display the screen and update the table when we are in KD_CSTATE_HIGHSCORE mode.
	*/
	bool DisplayHighScoreState();


	/**
		Method called when Quitting the controller.
	*/
	virtual bool Quit();

	void LoadSprites();
	void UnloadSprites();

	virtual bool OnEnable();
	virtual bool OnDisable();
};

#endif
