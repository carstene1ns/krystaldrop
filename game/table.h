#ifndef Table_H
#define Table_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include "deque"
using namespace std;

#define KD_HORIZONTAL_BAR 0
#define KD_VERTICAL_BAR 1
#define KD_UPPER_LEFT_BAR 2
#define KD_UPPER_RIGHT_BAR 3

class KD_Sprite;
class KD_SpriteInstance;
class KD_Gem;
class KD_Parameters;
class KD_Set;
class TACCRes;

/**
	Number of different gems existing.
  */
#define KD_NB_GEMS 6
#define KD_BLUE 0
#define KD_GREEN 1
#define KD_RED 2
#define KD_YELLOW 3

/**
	Class containing a table (the balls + the clown + the score and all the intersting stuff).
  */
class KD_Table
{
private:
	int width;
	int height;
	int xPos,yPos;

	/// gem width in pixels
	int gemWidth;
	/// gem height in pixels
	int gemHeight;

	KD_Sprite *border[4];
	KD_Sprite *clownSpr;
	KD_SpriteInstance *clown;

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
		Time in millisecond when the previous frame as been displayed. Used for the clown speed.
	*/
	int ticks;

	KD_Parameters* param;

	/// The structure holding the gems and all the logical part of krystal drop.
	KD_Set* set;

	/// The sprites of each kind of gem.
	KD_Sprite *gem[KD_NB_GEMS];

	/// Gems that will appear are stored here.
	deque<unsigned char *> gemsToCome;

	/// In each column, we have to know what is the last gem that came.
	int *gemThatCame;

	/**
		At the end of displaying the gemsToCome, shall we start again or take the gems at random?
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
		Method used when updating the display to tell the KD_Set that we have some lines to add....
	*/
	void tryAddGemsToKDSet();

	/**
		Get a gemat random given the random parameters passed.
	*/
	unsigned char getRandomGem();

public:
	KD_Table();
	~KD_Table();

	/**
		Set the width of the field to width.
		width id expressed in number of balls.
	*/
	void setWidth(int width);

	/**
		Set the height of the field to height.
		height id expressed in number of balls.
	*/
	void setHeight(int height);

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


	//{
	/**
		Sets the sprites for the border of the field.
	*/
	void setHorizontalBar(KD_Sprite *spr);
	void setVerticalBar(KD_Sprite *spr);
	void setUpperLeftBar(KD_Sprite *spr);
	void setUpperRightBar(KD_Sprite *spr);
	//}

	/**
		Sets the clown sprite
	*/
	void setClownSprite(KD_Sprite *spr);

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
	void setLoopGems(bool loopGems);

	//{
	/**
		Loads the set of gems that are going to be played from a file.
		The structure of the file is the following one:
		each line in the file is a row in the table.
		put a "b" for a blue gem
		put a "g" for a green gem
		put a "r" for a red gem
	*/
	signed loadGemsToCome(char *fileName);
	signed loadGemsToCome(TACCRes *accFile, char *fileName);
	//}

	/**
		Displays the whole table.
	*/
	void Display();

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
		Initialises the KD_Parameters and the KD_Set.
	*/
	void Init();

	/**
		Deinitialises the KD_Parameters and the KD_Set.
	*/
	void deInit();

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
		Take gems from the screen and puts them in the clown hand.
	*/
	void takeGems();

	/**
		Drop gems from the clown hand to the screen.
	*/
	void dropGems();
};

#endif
