#ifndef Table_H
#define Table_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include "deque"
using namespace std;

#include "../Names.h"

#include "Character.h"
#include "../../KDpp/Tools/FilePath.h"

#define KD_HORIZONTAL_BAR 0
#define KD_VERTICAL_BAR 1
#define KD_UPPER_LEFT_BAR 2
#define KD_UPPER_RIGHT_BAR 3
#define KD_LEFTDOOR 4
#define KD_RIGHTDOOR 5
#define KD_BOTTOM_BAR 6

#define KD_NB_SPRITE_FOR_BORDER 10

class TACCRes;
class KD_Gem;
class KD_Parameters;
class KD_Set;
class KD_Sound;
class KD_Sprite;
class KD_SpriteInstance;


/**
	Moves (anims) of the clown
  */
#define KD_CLOWN_IDLE 0
#define KD_CLOWN_RIGHT 1
#define KD_CLOWN_LEFT 2


/**
	Class containing a table (the balls + the clown + the score and all the intersting stuff).
  */
class KD_Table
{ protected: 
  // temp
  public:

	/**
		The resources related to the character (background sprite, chibi and voices)
	*/
	KD_Character character;

	/**
		The number of clashes (combo number).
	*/
    unsigned clash_count;

	/**
		The number of clashes at the end of a combo.
		0 otherwise.
	*/
	unsigned clash_count_finished;

	/**
		True is there has been a succesful clash test on last frame
	*/
	bool hasClashed;

	/**
		Well it is the player score... that's it!
	*/
	int score;

	/**
		The number of gems that has been removed on this frame.
		Useful to compute the score!
	*/
	int nbGemsRemoved;

	/**
		The total number of gems that clashed since the beginning of the party.
		Useful to know the current level in Survival Mode.
	*/
	int nbGemsDropped;

	/**
		True if the clown is holding some gems.
		False if its hands are empty.
	*/
	bool isHoldingGems;

	int width;
	int height;
	int xPos,yPos;

	/// gem width in pixels
	int gemWidth;
	/// gem height in pixels
	int gemHeight;

	KD_Sprite *border[KD_NB_SPRITE_FOR_BORDER];

	KD_SpriteInstance *leftDoor;
	KD_SpriteInstance *rightDoor;
	KD_SpriteInstance *lineSpriteInstance;

	/**
		Column of the clown.
	*/
	int clownPos;

	/**
		Position of the clown in pixels from the left border of the table.
	*/
	float clownPosInPixels;

	/**
		Speed of the clown.
		It is expressed in Columns per seconds.
		If the speed asked by the player is greater, no problem, the clown will jump to the next last column.
	*/
	float clownSpeed;

	/**
		Time in millisecond when the previous frame as been Displayed. Used for the clown speed.
	*/
	int ticks;
    
	KD_Parameters* param;

	/// The structure holding the gems and all the logical part of krystal drop.
	KD_Set* set;

	/// The sprites of each kind of gem.
	KD_Sprite *gem[KD_GEM_NB_KINDS];

	/// Gems that will appear are stored here.
	deque<unsigned char *> gemsToCome;

	/// In each column, we have to know what is the last gem that came.
	int *gemThatCame;

	/**
		At the end of Displaying the gemsToCome, shall we start again or take the gems at random?
		If true, we will loop the gems, else we will continue at random
	*/
	bool loopGems;

	/**
		Numbers of gems that are still to be added on the screen in each column.
	*/
	int *nbGemsToDrop;

	/**
		Very next row of Gems to be added.
	*/
	KD_Gem **rowToAdd;

	/**
		Probability that a gem will be obtained at random.
		The greater the number, the more likely the gem will be.
	*/
	unsigned int gemProbability[KD_GEM_NB_KINDS];

	/**
		Sum of the probabilities.
	*/
	unsigned int probabilitySum;

	/**
		Sounds made when a ball is ploping
	*/
#ifndef NO_SOUND
	KD_Sound *plopSound;
	KD_Sound *chocSound;
	KD_Sound *gemsUpSound;
	KD_Sound *gemsDownSound;
	KD_Sound **clashSound;
#endif    

	/**
		A table of pointers to gem. It is only used when the game is ended, to provide easily the small anim.
	*/
	KD_Gem **gemTableOnFinish;

	//{
	/**
		The speed of each gem when the game finishes. Used to drop all the gems when the game finishes.
		The speed is expressed in pixels per seconds.
	*/
	float *xSpeedOnFinish;
	float *ySpeedOnFinish;
	//}

	//{
	/**
		The position of each gem when the game finishes. Used to drop all the gems when the game finishes.
		The position is needed in float to avoid rounding errors.
	*/
	float *xGemOnFinish;
	float *yGemOnFinish;
	//}

	/**
		The number of gems in the final animation (on win or on loose).
	*/
	int nbGemsOnFinish;

	/**
		Method used when updating the Display to tell the KD_Set that we have some lines to add....
	*/
	void tryAddGemsToKDSet();

	/**
		Get a gem at random given the random parameters passed.
	*/
	unsigned char getRandomGem();

	/**
		If true, then the doors on the left and right of the Table will be activated and Displayed.
		If false, they won't even be Displayed.
	*/
	bool doors;

	/**
		This method domutes the new score from the numbers of ball that have disappeared and from the "combo meter".
	*/
	void computeScore();

	/**
		Displays the borders of the table.
	*/
	void DisplayBorders();

	/**
		Displays the gems in the table.
	*/
	void DisplayGems();

	/**
		Displays the clown.
		msElapsed: number of milliseconds elapsed since last frame.
	*/
	void DisplayClown(int msElapsed);

	/**
		Displays all the gems when we loose, dropping to the bottom of the screen and bouncing on it.
	*/
	void DisplayGemsOnLose();

	/**
		Displays all the gems when we win, going up until they are out of screen.
	*/
	void DisplayGemsOnWin();

	/**
		Prepares the set of animation that is going to be launched because the player has lost or because he won.
		This method is called by prepareLoose and prepareWin
	*/
	bool prepareFinish();

	/**
		Sets the clown position. Should be used only on table Initialisation.
	*/
	bool setClownPos(int clownPos);

public:
	KD_Table();
	~KD_Table();

	/**
		Initialize the parameters of KD_Table with zeroes.
	*/
	void Init();

	/**
		Desallocate all the memory used by the tables in KD_Table.
	*/
	void desalloc();

	/**
		Set the width of the field to width.
		width id expressed in number of gems.
	*/
	void setWidth(int width);

	/**
		Set the height of the field to height.
		height id expressed in number of gems.
	*/
	void setHeight(int height);

	/**
		Get the width of the field to width.
		width id expressed in number of balls.
	*/
	int getWidth();

	/**
		Get the height of the field to height.
		height id expressed in number of balls.
	*/
	int getHeight();

	/**
		Set the position of the Table to (x,y)
	*/
	void setPosition(int x, int y);

	/**
		Set the width of the gems in pixel
	*/
	void setGemWidth(int gemWidth);

	/**
		Set the height of the gems in pixel
	*/
	void setGemHeight(int gemHeight);

	/***
		If true, then the doors on the left and right of the Table will be activated and Displayed.
		If false, they won't even be Displayed.
	*/
	void activateDoors(bool doors);

	//{
	/**
		Sets the sprites for the border of the field.
	*/
	void KD_Table::setAllBorders(KD_Sprite *spr[KD_NB_SPRITE_FOR_BORDER]);
	void setHorizontalBar(KD_Sprite *spr);
	void setVerticalBar(KD_Sprite *spr);
	void setUpperLeftBar(KD_Sprite *spr);
	void setUpperRightBar(KD_Sprite *spr);
	void setLeftDoor(KD_Sprite *spr);
	void setRightDoor(KD_Sprite *spr);
	void setBottomBar(KD_Sprite *spr);
	//}

	/// Sets the sprite for the line going from the clown to the field.
	void setLineSprite(KD_Sprite *lineSprite);

	/**
		Sets the clown speed.
	*/
	void setClownSpeed(float clownSpeed);

	/**
		Set the sprites for the gems
	*/
	void setGems(KD_Sprite **gems);

	/**
		Set if the table must loop once all the gems that where planned are gone.
	*/
	void SetLoopGems(bool loopGems);

	//{
	/**
		Loads the set of gems that are going to be played from a file.
		The structure of the file is the following one:
		each line in the file is a row in the table.
		"b" is for a blue gem, "g" green, "r" red, "y" yellow
	*/
	signed loadGemsToCome(const char *fileName);
	signed loadGemsToCome(TACCRes *accFile, const char *fileName);
	//}

	/**
		Displays the whole table while playing.
	*/
	void Display();

	/**
		Initialises the KD_Parameters and the KD_Set.
		(Passes the arguments hold in KD_Table to KD_Parameters and KD_Set).
	*/
	void InitSet();

	/**
		DeInitialises the KD_Parameters and the KD_Set.
	*/
	void deInit();

	/**
		Resets the table: keeps the parameters, but removes all the gems.
	*/
	void resetTable();

	/**
		Moves the clown to the left
	*/
	void MoveLeft();

	/**
		Moves the clown to the right
	*/
	void MoveRight();

	/**
		Adds a gem in the column number "column"
	*/
	void addGemInColumn(int column);

	/**
		Add a full line (it adds a gem in each column).
	*/
	void addLine();

	/**
		Returns true if some gems are still to be added in the table.
	*/
	bool isAddingGems();

	/**
		Take gems from the screen and puts them in the clown hand.
	*/
	void takeGems();

	/**
		Drop gems from the clown hand to the screen.
	*/
	void dropGems();

	/**
		Set the probability to get a gem at random.
		The greater the number, the more likely the gem will be.
	*/
	void setGemProbability(int gemKind, unsigned int probability);

	/**
		Sets the sound made when a ball is ploping.
	*/
	void setPlopSound(KD_Sound *plopSound);

	/**
		Sets the sound made when gems are going down.
	*/
	void setGemsDownSound(KD_Sound *gemsDownSound);

	/**
		Sets the sound made when gems are going up.
	*/
	void setGemsUpSound(KD_Sound *gemsUpSound);

	/**
		Sets the sound made when gems hit other gems.
	*/
	void setChocSound(KD_Sound *chocSound);

	/**
		Sets the sounds made when combos are done.
	*/
	void setClashSounds(KD_Sound **clashSound);

	/**
		Returns the current number of clashes
	*/
	int getClashCount();

	/**
		Returns true is there has been a succesful clash test on last frame
	*/
	bool getHasClashed();

	/**
		Returns the number of clashes that occured and that must be sent to the opponent.
		This is useful in 2 players mode.
		So this method will return 0, except at the end of a combo where it will return the number of clashes in the combo.
	*/
	int getClashCountFinished();

	/**
		Returns the number of gems on the table.
	*/
	int getGemCount();

	/**
		Returns the player score.
	*/
	int getScore();

	/**
		Return the total number of gems that clashed since the beginning of the party.
		Useful to know the current level in Survival Mode.
	*/
	int getNbGemsDropped();

	/**
		Returns true if the clown is holding some gems, false otherwise.
	*/
	bool getIsHoldingGems();

	/**
		Returns the maximum height of the table in number of gems.
	*/
	int getMaxHeight();

	/**
		Returns if it is time for us to test for the maximum height in order to know if we
		have lost or if we have to add a few more gems in SurvivalMode.
	*/
	bool isTestMaxHeightNeeded();

	/**
		Prepares the set of animation that is going to be launched because the player has lost
		This method calls prepareFinish and set the right animation for the clown
	*/
	bool prepareLose();

	/**
		Displays the whole table when the player lost.
	*/
	void DisplayOnLose();

	/**
		Prepares the set of animation that is going to be launched because the player has won
		This method calls prepareFinish and set the right animation for the clown
	*/
	bool prepareWin();

	/**
		Displays the whole table when the player won.
	*/
	void DisplayOnWin();

	/**
		Loads the character's data
	*/
	bool LoadCharacter(const KD_FilePath &resourceFilePath, const KD_FilePath &actionFilePath);

	/**
		Unloads the character's data
	*/
	bool UnloadCharacter();

	/**
		Makes the character behave in a way described by actionNumber.
		Possibloe values:
			KD_ATTACK
			KD_STRONGATTACK
			KD_ATTACKED
			KD_DANGER
			KD_WINNING
			KD_LOOSING
	*/
	void TriggerCharacterAction(int actionNumber);


};

#endif
