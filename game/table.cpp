#include "table.h"

#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../video/image.h"


#include <assert.h>

KD_Table::KD_Table()
{

}

KD_Table::~KD_Table()
{

}

void KD_Table::setWidth(int width)
{
	this->width = width;
}

void KD_Table::setHeight(int height)
{
	this->height = height;
}

void KD_Table::setPosition(int x, int y)
{
	xPos=x;
	yPos=y;
}

void KD_Table::setGemSize(int gemSize)
{
	this->gemSize = gemSize;
}

void KD_Table::setHorizontalBar(KD_Sprite *spr)
{
	assert(spr);
	border[KD_HORIZONTAL_BAR] = spr;
}

void KD_Table::setVerticalBar(KD_Sprite *spr)
{
	assert(spr);
	border[KD_VERTICAL_BAR] = spr;
}

void KD_Table::setUpperLeftBar(KD_Sprite *spr)
{
	assert(spr);
	border[KD_UPPER_LEFT_BAR] = spr;
}

void KD_Table::setUpperRightBar(KD_Sprite *spr)
{
	assert(spr);
	border[KD_UPPER_RIGHT_BAR] = spr;
}

void KD_Table::setClownSprite(KD_Sprite *spr)
{
	clownSpr = spr;
}

void KD_Table::DisplayBorders()
{
	int x = xPos;
	
	int y = yPos - border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getHeight();

	while (x < xPos + width*gemSize)
	{
		border[KD_HORIZONTAL_BAR]->Display(x,y,0,0);
		x += border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getWidth();
	}

	x = xPos - border[KD_VERTICAL_BAR]->getAnim(0)->getImage(0)->getWidth();
	y = yPos;

	while (y < yPos + height*gemSize)
	{
		border[KD_VERTICAL_BAR]->Display(x,y,0,0);
		border[KD_VERTICAL_BAR]->Display(xPos+width*gemSize,y,0,0);
		y += border[KD_VERTICAL_BAR]->getAnim(0)->getImage(0)->getHeight();
	}

	border[KD_UPPER_LEFT_BAR]->Display(xPos-border[KD_UPPER_LEFT_BAR]->getAnim(0)->getImage(0)->getWidth(),yPos-border[KD_UPPER_LEFT_BAR]->getAnim(0)->getImage(0)->getHeight(),0,0);

	border[KD_UPPER_RIGHT_BAR]->Display(xPos+width*gemSize,yPos-border[KD_UPPER_RIGHT_BAR]->getAnim(0)->getImage(0)->getHeight(),0,0);

}
