#include "table.h"

#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../video/image.h"
#include "parameter.h"
#include "set.h"

#include <assert.h>

KD_Table::KD_Table()
{
	set = 0;
	param = 0;
	clown = 0;
	setClownSpeed(20);
	ticks = SDL_GetTicks();
}

KD_Table::~KD_Table()
{

}

void KD_Table::setWidth(int width)
{
	this->width = width;
	clownPos = width / 2;
	if (set)
		set->pos = clownPos;
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

void KD_Table::setGemWidth(int gemWidth)
{
	this->gemWidth = gemWidth;
}

void KD_Table::setGemHeight(int gemHeight)
{
	this->gemHeight = gemHeight;
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
	if (clown) delete clown;
	clownSpr = spr;
	clown = new KD_SpriteInstance(spr);
	clownPosInPixels = clownPos*gemWidth;
	clown->x = xPos+clownPosInPixels+gemWidth/2;
	clown->y = yPos+(height+1)*gemHeight;
}

void KD_Table::setClownSpeed(float clownSpeed)
{
	this->clownSpeed = clownSpeed;
}

void KD_Table::setGems(KD_Sprite **gems)
{
	for (int i=0; i<KD_NB_GEMS; i++)
		gem[i] = gems[i];
}

void KD_Table::Display()
{
	int old_ticks = ticks;
	ticks = SDL_GetTicks();

	DisplayClown(ticks-old_ticks);
	DisplayGems();
	DisplayBorders();
}

void KD_Table::DisplayBorders()
{
	int x = xPos;
	
	int y = yPos - border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getHeight();

	while (x < xPos + width*gemWidth)
	{
		border[KD_HORIZONTAL_BAR]->Display(x,y,0,0);
		x += border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getWidth();
	}

	x = xPos - border[KD_VERTICAL_BAR]->getAnim(0)->getImage(0)->getWidth();
	y = yPos;

	while (y < yPos + height*gemHeight)
	{
		border[KD_VERTICAL_BAR]->Display(x,y,0,0);
		border[KD_VERTICAL_BAR]->Display(xPos+width*gemWidth,y,0,0);
		y += border[KD_VERTICAL_BAR]->getAnim(0)->getImage(0)->getHeight();
	}

	border[KD_UPPER_LEFT_BAR]->Display(xPos-border[KD_UPPER_LEFT_BAR]->getAnim(0)->getImage(0)->getWidth(),yPos-border[KD_UPPER_LEFT_BAR]->getAnim(0)->getImage(0)->getHeight(),0,0);

	border[KD_UPPER_RIGHT_BAR]->Display(xPos+width*gemWidth,yPos-border[KD_UPPER_RIGHT_BAR]->getAnim(0)->getImage(0)->getHeight(),0,0);

}

void KD_Table::DisplayGems()
{
	set->Update();
  
	if (param->IsNeedClashTest())
    /* and we can..*/
	{
		set->TestBurstStart ();
		param->ClearNeedClashTest();
	}
  
	KD_Gem* gem= set->GetFirstGem();
	while (gem!= NULL)
	{
		gem->Display();
		gem= set->GetNextGem();
	}
}

void KD_Table::DisplayClown(int msElapsed)
{
	if (clownPosInPixels < clownPos*gemWidth)
	{
		clownPosInPixels += gemWidth*msElapsed*clownSpeed/1000.0f;

		if (clownPosInPixels > clownPos*gemWidth)
			clownPosInPixels = clownPos*gemWidth;

		clown->x = xPos+clownPosInPixels+gemWidth/2;
	}
	else if (clownPosInPixels > clownPos*gemWidth)
	{
		clownPosInPixels -= gemWidth*msElapsed*clownSpeed/1000.0f;

		if (clownPosInPixels < clownPos*gemWidth)
			clownPosInPixels = clownPos*gemWidth;

		clown->x = xPos+clownPosInPixels+gemWidth/2;
	}

	
	clown->DisplayCentered();
}

void KD_Table::Init()
{
	/* debug */
	param= new KD_Parameters();
	param->SetVideoParameters (gemHeight, gemWidth, gemHeight*height, yPos);
	param->SetGameParameters (3, 0, 1, 1, -1, -1);

#define MAX_IN_HAND 14
	
	set= new KD_Set(width, height, MAX_IN_HAND, param);
	set->pos = clownPos;
	/* */

/*	void SetGameParameters (short Line_Down_Speed, short Line_Down_Accel,
                           short Take_Hand_Speed, short Take_Hand_Accel,
                           short Drop_Hand_Speed, short Drop_Hand_Accel);
   void SetVideoParameters (short Height_Gem_In_Pixel, short Width_Gem_In_Pixel,
                            short Height_Field_In_Pixel, short Offset_Field_In_Pixel);*/

}

void KD_Table::deInit()
{
//	delete set;
	delete param;
}

void KD_Table::MoveLeft()
{
	clownPosInPixels = clownPos*gemWidth;
	if (clownPos>0)
		clownPos--;
	set->MoveLeft();
}

void KD_Table::MoveRight()
{
	clownPosInPixels = clownPos*gemWidth;
	if (clownPos<width-1)
		clownPos++;
	set->MoveRight();
}
