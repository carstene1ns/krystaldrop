#ifndef Table_H
#define Table_H

#define KD_HORIZONTAL_BAR 0
#define KD_VERTICAL_BAR 1
#define KD_UPPER_LEFT_BAR 2
#define KD_UPPER_RIGHT_BAR 3

class KD_Sprite;
class KD_SpriteInstance;
class KD_Parameters;
class KD_Set;

/**
	Number of different gems existing.
  */
#define KD_NB_GEMS 6
#define KD_BLUE 0

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
	KD_Set* set;

	KD_Sprite *gem[KD_NB_GEMS];

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
};

#endif
