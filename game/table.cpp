#include <assert.h>

#include "parameter.h"
#include "set.h"
#include "table.h"
#include "../sound/sound.h"
#include "../util/logfile.h"
#include "../util/textfile.h"
#include "../video/font.h"
#include "../video/Display.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../video/image.h"
#include "../video/gem.h"


#define KD_WRONG_GEM_FILE -110;

KD_Table::KD_Table()
{
	Init();
}

KD_Table::~KD_Table()
{
	desalloc();
}

void KD_Table::Init()
{
	set = 0;
	param = 0;
	clown = 0;
	setClownSpeed(20);
	ticks = SDL_GetTicks();
	gemThatCame=0;
	loopGems=0;
	nbGemsToDrop=0;
	rowToAdd=0;
	clash_count= 0;
	clash_count_finished=0;
	hasClashed = false;
	isHoldingGems=false;
	score=0;
	nbGemsRemoved=0;
	nbGemsDropped=0;

	gemTableOnFinish=0;
	xSpeedOnFinish=0;
	ySpeedOnFinish=0;
	xGemOnFinish=0;
	yGemOnFinish=0;
	nbGemsOnFinish=0;

	for (int i=0; i<KD_NB_GEMS; i++)
		gemProbability[i]=0;

	probabilitySum=0;
    
#ifndef NO_SOUND
	plopSound=0;
#endif    

	doors=0;
	leftDoor=0;
	rightDoor=0;
}

void KD_Table::desalloc()
{
	if (gemThatCame)
	{
		delete[] gemThatCame;
		gemThatCame = 0;
	}

	if (nbGemsToDrop)
	{
		delete[] nbGemsToDrop;
		nbGemsToDrop = 0;
	}

	for (unsigned int i=0; i<gemsToCome.size(); i++)
	{
		delete[] gemsToCome[i];
		gemsToCome[i]=0;
	}
	gemsToCome.clear();

	if (rowToAdd)
	{
		delete[] rowToAdd;
		rowToAdd = 0;
	}

	if(gemTableOnFinish)
	{
		delete[] gemTableOnFinish;
		gemTableOnFinish = 0;
	}

	if(xSpeedOnFinish)
	{
		delete[] xSpeedOnFinish;
		xSpeedOnFinish = 0;
	}

	if(ySpeedOnFinish)
	{
		delete[] ySpeedOnFinish;
		ySpeedOnFinish = 0;
	}

	if(xGemOnFinish)
	{
		delete[] xGemOnFinish;
		xGemOnFinish = 0;
	}

	if(yGemOnFinish)
	{
		delete[] yGemOnFinish;
		yGemOnFinish = 0;
	}
}

void KD_Table::setWidth(int width)
{
	this->width= width;
	clownPos= width/ 2;

	if (gemsToCome.size() != 0)
	{
		printf("Warning! The table has been resized will there was data in the gemsToCome structure!\n");
		KD_LogFile::printf("Warning! The table has been resized will there was data in the gemsToCome structure!\n");
		assert(!gemsToCome.size());
	}

	if (gemThatCame)
		delete[] gemThatCame;
	gemThatCame = new int[width];

	
	if (nbGemsToDrop)
		delete[] nbGemsToDrop;
	nbGemsToDrop = new int[width];

	if (rowToAdd)
		delete[] rowToAdd;
	rowToAdd = new KD_Gem*[width];

	for (int i=0; i<width; i++)
	{
		nbGemsToDrop[i]=0;
		gemThatCame[i]=0;
		rowToAdd[i]=0;
	}
}


void KD_Table::setHeight(int height)
{
	this->height = height;
}

int KD_Table::getWidth()
{
	return width;
}

int KD_Table::getHeight()
{
	return height;
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

void KD_Table::activateDoors(bool doors)
{
	this->doors = doors;
}

void KD_Table::setAllBorders(KD_Sprite *spr[KD_NB_SPRITE_FOR_BORDER])
{
	for (int i=0; i<KD_NB_SPRITE_FOR_BORDER; i++)
		if (spr[i])
			border[i] = spr[i];
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

void KD_Table::setLeftDoor(KD_Sprite *spr)
{
	assert(spr);
	border[KD_LEFTDOOR] = spr;
	if (leftDoor)
		delete leftDoor;
	leftDoor = new KD_SpriteInstance(spr);
}

void KD_Table::setRightDoor(KD_Sprite *spr)
{
	assert(spr);
	border[KD_RIGHTDOOR] = spr;
	if (rightDoor)
		delete rightDoor;
	rightDoor = new KD_SpriteInstance(spr);
}

void KD_Table::setBottomBar(KD_Sprite *spr)
{
	assert(spr);
	border[KD_BOTTOM_BAR] = spr;
}

void KD_Table::setClownSprite(KD_Sprite *spr)
{
	if (clown) delete clown;
	clownSpr = spr;
	clown = new KD_SpriteInstance(spr);
	clownPosInPixels = clownPos*gemWidth;
	clown->x = xPos+(int)clownPosInPixels+gemWidth/2;
	clown->y = yPos+(height/*+1*/)*gemHeight;
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

void KD_Table::setLoopGems(bool loopGems)
{
	this->loopGems = loopGems;
}

signed KD_Table::loadGemsToCome(char *fileName)
{
	return loadGemsToCome(0, fileName);
}

signed KD_Table::loadGemsToCome(TACCRes *accFile, char *fileName)
{
	KD_TextFile file(accFile, fileName);

	///  100 columns for a table should never be reached.... we can hope.
#define BUF_COLUMNS_SIZE 100 /* ajout de krys */
	char buf[BUF_COLUMNS_SIZE];

	while (!file.isEOF())
	{
		sscanf(file.getPosition(),"%s",buf);

		if (buf[0]==0)
		{
			file.jumpLine();
			continue;
		}

		unsigned char *row = (unsigned char *) new char[width];

		for (int i=0; i<width; i++)
		{
			row[i]=buf[i];

			switch (buf[i])
			{	case 0:
					printf("Warning! the length of the %d line in the file %s is wrong. It should be %d characters long! Aborting loading of the file.\n", gemsToCome.size(), fileName, width);
					KD_LogFile::printf("Warning! the length of the %d line in the file %s is wrong. It should be %d characters long! Aborting loading of the file.\n", gemsToCome.size(), fileName, width);
					assert(0);
					delete[] row;
					return KD_WRONG_GEM_FILE;
				case 'b':
					row[i]=KD_GEM_N_BLUE;
					break;
				case 'g':
					row[i]=KD_GEM_N_GREEN;
					break;
				case 'r':
					row[i]=KD_GEM_N_RED;
					break;
				case 'y':
					row[i]=KD_GEM_N_YELLOW;
					break;
                /* ## add more gem types */
                default: 
                    assert (0);
                    break;
			}
		}
		gemsToCome.push_back(row);

		file.jumpLine();

	}

	return 0;
}

void KD_Table::Display()
{
  #ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
  #endif
  
	tryAddGemsToKDSet();
  #ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
  #endif

	int old_ticks = ticks;
	ticks = SDL_GetTicks();
   
	DisplayGems();
	DisplayBorders();
	DisplayClown(ticks-old_ticks);
}

void KD_Table::DisplayBorders()
{
	int x;
	int y;

	x = xPos-border[KD_BOTTOM_BAR]->getAnim(0)->getImage(0)->getWidth()/2;
	y = yPos + height*gemHeight/* - border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getHeight()*/;

	while (x < xPos + width*gemWidth)
	{
		border[KD_BOTTOM_BAR]->Display(x,y,0,0);
		x += border[KD_BOTTOM_BAR]->getAnim(0)->getImage(0)->getWidth();
	}

	x = xPos;
	y = yPos - border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getHeight();

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

	if (doors)
	{
		leftDoor->x = xPos-16;
		leftDoor->y = yPos + height*gemHeight - 64;
		leftDoor->Display();

		rightDoor->x = xPos + width*gemWidth -16;
		rightDoor->y = yPos + height*gemHeight - 64;
		rightDoor->Display();
	}

}

void KD_Table::DisplayGems()
{
#ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
#endif
 
	
	Display::setClipRect(xPos,yPos,xPos+width*gemWidth, yPos+(height/*+1*/)*gemHeight);

	hasClashed=false;
	clash_count_finished=0;
	int old_nb_gems = getGemCount();

	// Clear the flag saying we need to check the gemMaxHeight.
	//param->ClearCheckOverflow();
 

  if (param->IsRemoving())
  {
    set->RemoveGems();
  }
  else
  {
   if (set->GetMemo()->GetSize()!= 0)
   { if (set->IsUpFinished() && !(set->IsLineDown()) && set->TestBurstStart())
         { clash_count++;
           hasClashed= true;
         }
   }
   else
   if (set->IsUpFinished())
   { 
	   clash_count_finished = clash_count;
	   clash_count= 0; } 
  }

  set->Update();

#ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
#endif
  
  set->Display();

  nbGemsRemoved = old_nb_gems - getGemCount();

  computeScore();

  if (nbGemsRemoved>0) nbGemsDropped+= nbGemsRemoved;

   Display::setClipRect(0,0,Display::width,Display::height);
}


void KD_Table::DisplayClown(int msElapsed)
{
	if (clownPosInPixels < clownPos*gemWidth)
	{
		clownPosInPixels += gemWidth*msElapsed*clownSpeed/1000.0f;

		if (clownPosInPixels > clownPos*gemWidth)
			clownPosInPixels = clownPos*gemWidth;

		clown->x = xPos+(int)clownPosInPixels+gemWidth/2;
	}
	else if (clownPosInPixels > clownPos*gemWidth)
	{
		clownPosInPixels -= gemWidth*msElapsed*clownSpeed/1000.0f;

		if (clownPosInPixels < clownPos*gemWidth)
			clownPosInPixels = clownPos*gemWidth;

		clown->x = xPos+(int)clownPosInPixels+gemWidth/2;
	}

	
	clown->Display (KD_SPRITE_CENTERED_HORIZ);
    set->GetHand()->Display(xPos + clownPosInPixels, yPos+height*gemHeight-3*gemHeight/2+3);
}

void KD_Table::InitSet()
{
	/* debug */
	param= new KD_Parameters();
	param->SetVideoParameters (gemHeight, gemWidth, gemHeight*height, xPos, yPos);
	param->SetGameParameters (3, 0, -2, 0, 3, 5, -3, -1);
/*SetGameParameters (Line_Down_Speed, Line_Down_Accel, Gem_Up_Speed,    Gem_Up_Accel,
                     Take_Hand_Speed, Take_Hand_Accel, Drop_Hand_Speed, Drop_Hand_Accel);*/            

#define MAX_IN_HAND 14
	
	set= new KD_Set(width, height, MAX_IN_HAND, param);
}

void KD_Table::deInit()
{
	// delete set was commented.... why?
	delete set;
	delete param;
}

void KD_Table::MoveLeft()
{
	clownPosInPixels = clownPos*gemWidth;
	if (clownPos>0)
	{
		if(!isHoldingGems)
			clown->setAnim(KD_CLOWN_LEFT);
		clownPos--;
	}
	else if (clownPos==0 && doors==true)
	{
		if(!isHoldingGems)
			clown->setAnim(KD_CLOWN_LEFT);
		clownPos = width-1;
		clownPosInPixels = clownPos*gemWidth + gemWidth/2;
	}
}

void KD_Table::MoveRight()
{
	clownPosInPixels = clownPos*gemWidth;
	if (clownPos<width-1)
	{
		if(!isHoldingGems)
			clown->setAnim(KD_CLOWN_RIGHT);
		clownPos++;
	}
	else if (clownPos==width-1 && doors==true)
	{
		if(!isHoldingGems)
			clown->setAnim(KD_CLOWN_RIGHT);
		clownPos = 0;
		clownPosInPixels = - gemWidth/2;
	}
}

void KD_Table::addGemInColumn(int column)
{
	nbGemsToDrop[column]++;
}

void KD_Table::addLine()
{
	for (int i=0; i<width; i++)
		addGemInColumn(i);
}

bool KD_Table::isAddingGems()
{
	for (int i=0; i<width; i++)
		if (nbGemsToDrop[i]!=0)
			return true;

	return false;
}

void KD_Table::tryAddGemsToKDSet()
{
	// We're not trying to add lines if a combo is done at the same time:
	if (getClashCount() != 0) return;

	int i;
	unsigned char gemToAdd;
	bool mustWeTry = false;
	for (i=0; i<width; i++)
	{
		rowToAdd[i]=0;
		if (nbGemsToDrop[i]!=0)
		{
			mustWeTry=true;

			if (gemThatCame[i] == (int)gemsToCome.size()-1)
			{
				if (loopGems) gemThatCame[i]=0;
				else 
				{
					unsigned char randomGem = getRandomGem();
					rowToAdd[i] = new KD_Gem(set, gem[randomGem],randomGem);
                    assert (rowToAdd[i]);
			/*		goto endFor;*/ continue;
				}
			}

			gemToAdd  = gemsToCome[gemThatCame[i]][i];
			rowToAdd[i] = new KD_Gem(set,gem[gemToAdd],gemToAdd);
            assert (rowToAdd[i]);
/*endFor:;*/
		}        
	}
    
  #ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
  #endif
    

	if (!mustWeTry) return;

	signed ret = set->AddLineAtTop (rowToAdd);

	// If we can't add the line, please return.
	if (ret == KD_E_ADDIMPOSSIBLE) return;

	// If we can add the line, then update gemThatCame to point to the next one.
	for (i=0; i<width; i++)
	{
		if (nbGemsToDrop[i]!= 0)
		{
			if (gemThatCame[i] != (int)gemsToCome.size()-1)
				gemThatCame[i]++;

			nbGemsToDrop[i]--;
		}
	}

#ifndef NO_SOUND    
	plopSound->PlaySound();
#endif    
}

unsigned char KD_Table::getRandomGem()
{ int i;
	
	int value = (int)(((float)rand())/((float)RAND_MAX)*((float)probabilitySum));
	int sum=0;

	for (i= 0; i<KD_NB_GEMS; i++)
	{
		sum += gemProbability[i];
		if (value<=sum) break;
	}

	return i;
}

void KD_Table::takeGems()
{
	signed res = set->TakeGems (clownPos);

	// If can't take gems:
	if (res!=0/*KD_E_HANDINCOMPATIBLE*/)
	{
		clown->setAnim(3);
	}
	else
	{
		clown->setAnim(5);
		isHoldingGems=true;
	}
}

void KD_Table::dropGems()
{
	signed res = set->DropGems (clownPos);

	// If there is nothing in our hand...
	if (res == KD_E_HANDEMPTY)
	{
		clown->setAnim(3);
	}
	else
	{
		clown->setAnim(6);
		isHoldingGems=false;
	}
}

bool KD_Table::setGemProbability(int gemKind, unsigned int probability)
{
	if (gemKind<0 || gemKind>=KD_NB_GEMS) return false;

	probabilitySum -= gemProbability[gemKind];
	gemProbability[gemKind] = probability;
	probabilitySum += gemProbability[gemKind];

	return true;
}


void KD_Table::setPlopSound(KD_Sound *plopSound)
{
#ifndef NO_SOUND
	this->plopSound = plopSound;
#endif  
}

int KD_Table::getClashCount()
{
	return (int)clash_count;
}

bool KD_Table::getHasClashed()
{
	return hasClashed;
}

int KD_Table::getClashCountFinished()
{
	return clash_count_finished;
}

int KD_Table::getGemCount()
{
	assert(param);
	return param->Get_Gems_Count();
}

int KD_Table::getScore()
{
	return score;
}

void KD_Table::computeScore()
{
	if (nbGemsRemoved<=0) return;

	/// Compute 2 to the power (clash_count)
	int pow=1;
	for (unsigned i=0; i< clash_count; i++)
		pow *= 2;

	score += nbGemsRemoved * pow;
}

int KD_Table::getNbGemsDropped()
{
	return nbGemsDropped;
}

bool KD_Table::getIsHoldingGems()
{
	return isHoldingGems;
}

int KD_Table::getMaxHeight()
{
	return set->GetMaxHeight();
}

bool KD_Table::isTestMaxHeightNeeded()
{
  if (param->NeedCheckOverflow()== 1 && set->GetMemo()== 0 && set->IsUpFinished()== 1)
      param->ClearCheckOverflow();

	if (param->NeedCheckOverflow()== 0 || getClashCount()> 0 || set->GetMemo()->GetSize()> 0)
		return false;
	else
		return true;
	//return param->NeedCheckOverflow();
}

bool KD_Table::prepareFinish()
{
	gemTableOnFinish = new KD_Gem*[getGemCount()];
	xGemOnFinish = new float[getGemCount()];
	yGemOnFinish = new float[getGemCount()];
	
	//for (int i=0; i<getGemCount(); i++)
	//{
	//}

	int index =0;

	for (int j=0; j<width; j++)
	{
		KD_AnimatedRow *row = set->field[j];
		short* p_block= row->GetFirstBlock();
		short  nb= row->GetBlockNb(p_block);
		while (nb!= 0)
		{ 
			KD_Gem* gem= row->GetBlockGem(p_block, nb- 1);
			assert (gem);
    					 
			nb--;
			if (nb== 0)
			{
				if (row->IsLastBlock(p_block)) break;
				p_block= row->GetNextBlock(p_block);
				nb= row->GetBlockNb(p_block);
				assert (nb);
			}

			if (gem->getAnim() != 0) continue;

			gemTableOnFinish[index]=gem;
			xGemOnFinish[index]=(float)gem->x;
			yGemOnFinish[index]=(float)gem->y;

			index++;
		}

	}

	nbGemsOnFinish = index;


	// Now, gemTableOnFinish contains all the gems on the terrain.

	// Let's now choose an initial speed for each gem.

	xSpeedOnFinish = new float[getGemCount()];
	ySpeedOnFinish = new float[getGemCount()];

	for (int i=0; i<index; i++)
	{
		// Initial speed for each gem. The maximum initial speed will be 3*32 pixels/seconds.
		xSpeedOnFinish[i] = -gemWidth*40/2 + 40*gemWidth*(float)rand()/(float)RAND_MAX;
		ySpeedOnFinish[i] = -gemHeight*40/2 + 40*gemHeight*(float)rand()/(float)RAND_MAX;

	}

	return true;
}

void KD_Table::DisplayGemsOnLose()
{
	assert(gemTableOnFinish);

	float yAccel = 4* 240.0f;
	float amortissement = 0.6f;

	for (int i=0; i<nbGemsOnFinish ; i++)
	{
		ySpeedOnFinish[i] += yAccel*Display::timeElapsed;

		float x = /*(float) gemTableOnFinish[i]->x*/xGemOnFinish[i];
		float y = /*(float) gemTableOnFinish[i]->y*/yGemOnFinish[i];

		x += xSpeedOnFinish[i]*Display::timeElapsed;
		y += ySpeedOnFinish[i]*Display::timeElapsed;

		if (y-yPos > (height-1)*gemHeight)
		{
			y=yPos+(height-1)*gemHeight;

			ySpeedOnFinish[i] = -ySpeedOnFinish[i]*amortissement;
		}

		if (x-xPos < 0)
		{
			x=xPos;

			xSpeedOnFinish[i] = -xSpeedOnFinish[i]*amortissement;
		}
		else if (x-xPos > (width-1)*gemWidth)
		{
			x=xPos+(width-1)*gemWidth;

			xSpeedOnFinish[i] = -xSpeedOnFinish[i]*amortissement;
		}
		
		xGemOnFinish[i] = x;
		yGemOnFinish[i] = y;
		gemTableOnFinish[i]->x = (int)x;
		gemTableOnFinish[i]->y = (int)y;

		gemTableOnFinish[i]->Display();
	}

}

bool KD_Table::prepareLose()
{
	clown->setAnim(9);
	return prepareFinish();
}

void KD_Table::DisplayOnLose()
{
	int old_ticks = ticks;
	ticks = SDL_GetTicks();
    
	DisplayGemsOnLose();
	DisplayBorders();
	DisplayClown(ticks-old_ticks);	
}
