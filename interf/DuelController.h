#ifndef DuelController_H
#define DuelController_H

#include "Controller.h"
#include "../names.h"
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
class KD_InputBox;
class KD_TextEvent;


class KD_DuelController : public KD_Controller
{ 
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

	#define KD_DUEL_NB_PLAYERS 2

	/**
		Current counter for the number of clashes for both players.
	*/
	int clashCount[KD_DUEL_NB_PLAYERS];


	KD_Table table[KD_DUEL_NB_PLAYERS];

	/// Last time we added a line for both players.
	int last_line_added_time[KD_DUEL_NB_PLAYERS];

	/// The number of ticks to wait to add a line.
	int currentTimeBetweenLines;


	/**
		sprites of the border of the tables.
	*/
	KD_Sprite *border[KD_NB_SPRITE_FOR_BORDER];
	
	KD_Sprite *clown[KD_DUEL_NB_PLAYERS];
	KD_Sprite *gem[KD_GEM_NB_KINDS];

	KD_Sprite *characterSprite[KD_DUEL_NB_PLAYERS];
	KD_SpriteInstance *characterSpriteInstance[KD_DUEL_NB_PLAYERS];

	//KD_TextEvent *comboEvent;
	//KD_TextEvent *timer;
	
	/**
		The time at which the players started or finished the game.
		Used to change controller after a while.
		Used for the timer during the game too.
	*/
	int timeOfNewState;

	/**
		The background image
	*/
	KD_Image *background;

	#ifndef NO_SOUND
	KD_Sound *plopSound;
	#endif    

	/**
		The music to be played
	*/
    #ifndef NO_MUSIC
	KD_Music *music;
    #endif

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
		The method called to display the screen and update the table when we are in KD_CSTATE_PLAYING mode.
	*/
	bool displayPlayingState();

	/**
		Displays the table nbTable.
	*/
	bool displayTable(short nbTable);


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
