#ifndef Table_H
#define Table_H

#define KD_HORIZONTAL_BAR 0
#define KD_VERTICAL_BAR 1
#define KD_UPPER_LEFT_BAR 2
#define KD_UPPER_RIGHT_BAR 3

class KD_Sprite;
class KD_SpriteInstance;

/**
	Class containing a table (the balls + the clown + the score and all the intersting stuff).
  */
class KD_Table
{
private:
	int width;
	int height;
	int xPos,yPos;
	int gemSize;

	KD_Sprite *border[4];
	KD_Sprite *clownSpr;
	KD_SpriteInstance *clown;

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
		Set the size of the gems in pixel
	*/
	void setGemSize(int gemSize);


	//{
	/**
		Sets the sprites for the border of the field.
	*/
	void setHorizontalBar(KD_Sprite *spr);
	void setVerticalBar(KD_Sprite *spr);
	void setUpperLeftBar(KD_Sprite *spr);
	void setUpperRightBar(KD_Sprite *spr);
	//}

	void setClownSprite(KD_Sprite *spr);

	void DisplayBorders();


};

#endif
