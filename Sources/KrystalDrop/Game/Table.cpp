#include "../global.h"

#include "AnimRow.h"
#include "Hand.h"
#include "Memo.h"
#include "Parameter.h"
#include "Set.h"
#include "Table.h"
#include "../Video/Gem.h"
#include "../../KDpp/Sound/Sound.h"
#include "../../KDpp/Tools/Logfile.h"
#include "../../KDpp/Tools/Textfile.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"
#include "../../KDpp/Video/Image.h"

#define KD_WRONG_GEM_FILE -110

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
	setClownSpeed(20);
	ticks = Display::GetTicks();
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

	for (int i=0; i<KD_GEM_NB_KINDS; i++)
		gemProbability[i]=0;

	probabilitySum=0;
    
#ifndef NO_SOUND
	plopSound=0;
	chocSound=0;
	gemsUpSound=0;
	gemsDownSound=0;
	clashSound=0;
#endif    

	doors=0;
	leftDoor=0;
	rightDoor=0;
	lineSpriteInstance=0;
memset (border, 0, sizeof(border));
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

	if (rightDoor)
	{
		delete rightDoor;
		rightDoor = 0;
	}

	if (leftDoor)
	{
		delete leftDoor;
		leftDoor = 0;
	}

	if (lineSpriteInstance)
	{
		delete lineSpriteInstance;
		lineSpriteInstance = 0;
	}
}

void KD_Table::setWidth(int width)
{
	this->width= width;
	clownPos= width/ 2;

	if (gemsToCome.size() != 0)
	{
		printf("Warning! The table has been resized while there was data in the gemsToCome structure!\n");
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
	character.setBackGroundPos(x,y);
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

void KD_Table::setLineSprite(KD_Sprite *lineSprite)
{
	assert(lineSprite);
	if (lineSpriteInstance)
		delete lineSpriteInstance;

	lineSpriteInstance = new KD_SpriteInstance(lineSprite);

	lineSpriteInstance->setAnim(2);
}

void KD_Table::setClownSpeed(float clownSpeed)
{
	this->clownSpeed = clownSpeed;
}


void KD_Table::setGems(KD_Sprite **gems)
{
	for (int i= 0; i< KD_GEM_NB_KINDS; i++)
		gem[i]= gems[i];
}

void KD_Table::SetLoopGems(bool loopGems)
{
	this->loopGems = loopGems;
}

signed KD_Table::loadGemsToCome(const char *fileName)
{
	return loadGemsToCome(0, fileName);
}

signed KD_Table::loadGemsToCome(TACCRes *accFile, const char *fileName)
{
	KD_TextFile file(/*accFile,*/ fileName);

	///  100 columns for a table should never be reached.... we can hope.
#define BUF_COLUMNS_SIZE 100 /* ajout de krys */
	char buf[BUF_COLUMNS_SIZE];

	while (!file.IsEOF())
	{
		sscanf(file.GetPosition(),"%s",buf);

		if (buf[0]==0)
		{
			file.JumpLine();
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

		file.JumpLine();

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
	ticks = Display::GetTicks();
   
	character.DisplayBackground();
	DisplayBorders();
	DisplayClown(ticks-old_ticks);
	DisplayGems();
}

void KD_Table::DisplayBorders()
{
	int x;
	int y;

	x = xPos-border[KD_BOTTOM_BAR]->getAnim(0)->getImage(0)->GetWidth()/2;
	y = yPos + height*gemHeight/* - border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->getHeight()*/;

	while (x < xPos + width*gemWidth)
	{
		border[KD_BOTTOM_BAR]->Display(0,0,x,y);
		x += border[KD_BOTTOM_BAR]->getAnim(0)->getImage(0)->GetWidth();
	}

	x = xPos;
	y = yPos - border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->GetHeight();

	while (x < xPos + width*gemWidth)
	{
		border[KD_HORIZONTAL_BAR]->Display(0,0,x,y);
		x += border[KD_HORIZONTAL_BAR]->getAnim(0)->getImage(0)->GetWidth();
	}

	x = xPos - border[KD_VERTICAL_BAR]->getAnim(0)->getImage(0)->GetWidth();
	y = yPos;

	while (y < yPos + height*gemHeight)
	{
		border[KD_VERTICAL_BAR]->Display(0,0,x,y);
		border[KD_VERTICAL_BAR]->Display(0,0,xPos+width*gemWidth,y);
		y += border[KD_VERTICAL_BAR]->getAnim(0)->getImage(0)->GetHeight();
	}

	border[KD_UPPER_LEFT_BAR]->Display(0,0, xPos-border[KD_UPPER_LEFT_BAR]->getAnim(0)->getImage(0)->GetWidth(),yPos-border[KD_UPPER_LEFT_BAR]->getAnim(0)->getImage(0)->GetHeight());

	border[KD_UPPER_RIGHT_BAR]->Display(0,0, xPos+width*gemWidth,yPos-border[KD_UPPER_RIGHT_BAR]->getAnim(0)->getImage(0)->GetHeight());

	if (doors)
	{
		//leftDoor->x = xPos-16;
		//leftDoor->y = yPos + height*gemHeight - 64;
		leftDoor->Display(xPos-16, yPos + height*gemHeight - 64);

		//rightDoor->x = xPos + width*gemWidth -16;
		//rightDoor->y = yPos + height*gemHeight - 64;
		rightDoor->Display(xPos + width*gemWidth -16, yPos + height*gemHeight - 64);
	}

}

void KD_Table::DisplayGems()
{
#ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
#endif
 
	
	Display::SetClipRect(xPos,yPos,xPos+width*gemWidth, yPos+(height/*+1*/)*gemHeight);

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
         { 
			#ifndef NO_SOUND
				if (clashSound)
				{
					if (clash_count<KD_SND_NBCLASHSOUND)
						clashSound[clash_count]->PlaySound();
					else
						clashSound[clash_count-1]->PlaySound();
				}
			#endif    
			 
			 clash_count++;
			 hasClashed= true;
         }
   }
   else
   if (set->IsUpFinished())
   { 
     /* ## if special clashes */
     /* ## then ApplyClash */
     /* ## else */     
	   clash_count_finished = clash_count;
	   clash_count= 0; 
   } 
  }

  set->Update();

#ifdef DEBUG_SANITY_CHECK
  set->SanityCheck();
#endif
  
  set->Display();

  nbGemsRemoved = old_nb_gems - getGemCount();

  computeScore();

  if (nbGemsRemoved>0) nbGemsDropped+= nbGemsRemoved;

   Display::SetClipRect(0,0,Display::GetWidth(),Display::GetHeight());
}


void KD_Table::DisplayClown(int msElapsed)
{
	int xClown;

	if (clownPosInPixels < clownPos*gemWidth)
	{
		clownPosInPixels += gemWidth*msElapsed*clownSpeed/1000.0f;

		if (clownPosInPixels > clownPos*gemWidth)
			clownPosInPixels = (float) clownPos*gemWidth;

		//clown->x = xPos+(int)clownPosInPixels+gemWidth/2;
	}
	else if (clownPosInPixels > clownPos*gemWidth)
	{
		clownPosInPixels -= gemWidth*msElapsed*clownSpeed/1000.0f;

		if (clownPosInPixels < clownPos*gemWidth)
			clownPosInPixels = (float) clownPos*gemWidth;

		//clown->x = xPos+(int)clownPosInPixels+gemWidth/2;
		
	}
	xClown = xPos+(int)clownPosInPixels+gemWidth/2;
	
	//lineSpriteInstance->x = xPos+(int)clownPosInPixels+gemWidth/2;
	for (int i=0; i<height-1; i++)
	{
		//lineSpriteInstance->y = yPos+32*(i+1);
		//lineSpriteInstance->Display (KD_SPRITE_CENTERED_HORIZ);
		lineSpriteInstance->Display (xPos+(int)clownPosInPixels/*+gemWidth/2*/, yPos+32*i);
	}

	//clown->Display (KD_SPRITE_CENTERED_HORIZ);
	//clown->Display (xClown, yPos+(height/*+1*/)*gemHeight);
	character.DisplayChibi(xClown, yPos+height*gemHeight);
    set->GetHand()->Display( (int)(xPos+ clownPosInPixels), (int)(yPos+height*gemHeight-3*gemHeight/2+3));
}

void KD_Table::InitSet()
{
	/* debug */
	param= new KD_Parameters();
	param->SetVideoParameters (gemHeight, gemWidth, gemHeight*height, xPos, yPos);
	param->SetGameParameters (3, 0, -2, -1, 3, 5, -3, -1);
/*SetGameParameters (Line_Down_Speed, Line_Down_Accel, Gem_Up_Speed,    Gem_Up_Accel,
                     Take_Hand_Speed, Take_Hand_Accel, Drop_Hand_Speed, Drop_Hand_Accel);*/            

#define MAX_IN_HAND 14
	
	set= new KD_Set(width, height, MAX_IN_HAND, param);
}

void KD_Table::deInit()
{ short i;
  
	for (i= 0; i< width; i++) DELETE (rowToAdd[i]);
  
	// delete set was commented.... why?
	delete set;
	delete param;
}

void KD_Table::resetTable()
{
	ticks = Display::GetTicks();
	//gemThatCame=0;
	clash_count= 0;
	clash_count_finished=0;
	hasClashed = false;
	isHoldingGems=false;
	score=0;
	nbGemsRemoved=0;
	nbGemsDropped=0;

	setClownPos(width/2);

	deInit();

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

	for (int i=0; i<width; i++)
		nbGemsToDrop[i]=0;

	rowToAdd = new KD_Gem*[width];

	InitSet();
}

bool KD_Table::setClownPos(int clownPos)
{
	if (clownPos<0 || clownPos>=width)
		return false;

	this->clownPos = clownPos;
	clownPosInPixels = (float)clownPos*gemWidth;

	//clown->x = xPos+(int)clownPosInPixels+gemWidth/2;
	character.setChibiAnim(KD_CLOWN_IDLE);
    
    return true;
}

void KD_Table::MoveLeft()
{
	clownPosInPixels = (float) clownPos*gemWidth;
	if (clownPos>0)
	{
		if(!isHoldingGems)
			character.setChibiAnim(KD_CLOWN_LEFT);
			
		clownPos--;
	}
	else if (clownPos==0 && doors==true)
	{
		if(!isHoldingGems)
			character.setChibiAnim(KD_CLOWN_LEFT);
			
		clownPos = width-1;
		clownPosInPixels = (float) clownPos*gemWidth + gemWidth/2;
	}
}

void KD_Table::MoveRight()
{
	clownPosInPixels = (float) clownPos*gemWidth;
	if (clownPos<width-1)
	{
		if(!isHoldingGems)
			character.setChibiAnim(KD_CLOWN_RIGHT);
			
		clownPos++;
	}
	else if (clownPos==width-1 && doors==true)
	{
		if(!isHoldingGems)
			character.setChibiAnim(KD_CLOWN_RIGHT);
			
		clownPos = 0;
		clownPosInPixels = - (float) gemWidth/2;
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

	for (i= 0; i<KD_GEM_NB_KINDS; i++)
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
		character.setChibiAnim(3);
	}
	else
	{
		character.setChibiAnim(5);
		#ifndef NO_SOUND    
		if (gemsDownSound)
			gemsDownSound->PlaySound();
		#endif
		isHoldingGems=true;
		lineSpriteInstance->setAnim(1);
	}
}

void KD_Table::dropGems()
{
	signed res = set->DropGems (clownPos);

	// If there is nothing in our hand...
	if (res == KD_E_HANDEMPTY)
	{
		character.setChibiAnim(3);
	}
	else
	{
		character.setChibiAnim(6);
		#ifndef NO_SOUND    
		if (gemsUpSound)
			gemsUpSound->PlaySound();
		#endif
		isHoldingGems=false;
		lineSpriteInstance->setAnim(2);
	}
}

void KD_Table::setGemProbability(int gemKind, unsigned int probability)
{ assert (gemKind>=0 && gemKind< KD_GEM_NB_KINDS);

  probabilitySum -= gemProbability[gemKind];
  gemProbability[gemKind] = probability;
  probabilitySum += gemProbability[gemKind];
}


void KD_Table::setPlopSound(KD_Sound *plopSound)
{
#ifndef NO_SOUND
	this->plopSound = plopSound;
#endif  
}

void KD_Table::setGemsDownSound(KD_Sound *gemsDownSound)
{
#ifndef NO_SOUND
	this->gemsDownSound = gemsDownSound;
#endif  
}

void KD_Table::setGemsUpSound(KD_Sound *gemsUpSound)
{
#ifndef NO_SOUND
	this->gemsUpSound = gemsUpSound;
#endif  
}

void KD_Table::setChocSound(KD_Sound *chocSound)
{
#ifndef NO_SOUND
	this->chocSound = chocSound;
#endif  
}

void KD_Table::setClashSounds(KD_Sound **clashSound)
{
#ifndef NO_SOUND
	this->clashSound = clashSound;
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

	// Let's now choose an Initial speed for each gem.

	xSpeedOnFinish = new float[getGemCount()];
	ySpeedOnFinish = new float[getGemCount()];


	return true;
}

void KD_Table::DisplayGemsOnLose()
{
	assert(gemTableOnFinish);

	float yAccel = 4* 240.0f;
	float amortissement = 0.6f;

	for (int i=0; i<nbGemsOnFinish ; i++)
	{
		ySpeedOnFinish[i] += yAccel*Display::GetTimeElapsed();

		float x = /*(float) gemTableOnFinish[i]->x*/xGemOnFinish[i];
		float y = /*(float) gemTableOnFinish[i]->y*/yGemOnFinish[i];

		x += xSpeedOnFinish[i]*Display::GetTimeElapsed();
		y += ySpeedOnFinish[i]*Display::GetTimeElapsed();

		if (y-yPos > (height-1)*gemHeight)
		{
			y=(float) yPos+(height-1)*gemHeight;

			ySpeedOnFinish[i] = -ySpeedOnFinish[i]*amortissement;
		}

		if (x-xPos < 0)
		{
			x=(float) xPos;

			xSpeedOnFinish[i] = -xSpeedOnFinish[i]*amortissement;
		}
		else if (x-xPos > (width-1)*gemWidth)
		{
			x=(float) xPos+(width-1)*gemWidth;

			xSpeedOnFinish[i] = -xSpeedOnFinish[i]*amortissement;
		}
		
		xGemOnFinish[i] = x;
		yGemOnFinish[i] = y;
		gemTableOnFinish[i]->x = (int)x;
		gemTableOnFinish[i]->y = (int)y;

		gemTableOnFinish[i]->Display(gemTableOnFinish[i]->x, gemTableOnFinish[i]->y);
	}

}

bool KD_Table::prepareLose()
{
	character.setChibiAnim(9);

	if (!prepareFinish())
		return false;

	for (int i=0; i<nbGemsOnFinish; i++)
	{
		// Initial speed for each gem. The maximum Initial speed will be 3*32 pixels/seconds.
		xSpeedOnFinish[i] = -gemWidth*40/2 + 40*gemWidth*(float)rand()/(float)RAND_MAX;
		ySpeedOnFinish[i] = -gemHeight*40/2 + 40*gemHeight*(float)rand()/(float)RAND_MAX;

	}

	return true;
}

void KD_Table::DisplayOnLose()
{
	int old_ticks = ticks;
	ticks = Display::GetTicks();
    
	character.DisplayBackground();
	DisplayBorders();
	DisplayClown(ticks-old_ticks);	
	DisplayGemsOnLose();
}

void KD_Table::DisplayGemsOnWin()
{
	assert(gemTableOnFinish);

	Display::SetClipRect(xPos,yPos,xPos+width*gemWidth, yPos+(height/*+1*/)*gemHeight);

	float yAccel = - 40.0f;

	for (int i=0; i<nbGemsOnFinish ; i++)
	{
		float x = xGemOnFinish[i];
		float y = yGemOnFinish[i];

		ySpeedOnFinish[i] += yAccel*Display::GetTimeElapsed();

		if (y >= yPos-gemHeight)
			y += ySpeedOnFinish[i]*Display::GetTimeElapsed();
		
		xGemOnFinish[i] = x;
		yGemOnFinish[i] = y;
		gemTableOnFinish[i]->x = (int)x;
		gemTableOnFinish[i]->y = (int)y;

		gemTableOnFinish[i]->Display(gemTableOnFinish[i]->x, gemTableOnFinish[i]->y);
	}

	Display::SetClipRect(0,0,Display::GetWidth(),Display::GetHeight());
}

bool KD_Table::prepareWin()
{
	character.setChibiAnim(0);

	if (!prepareFinish())
		return false;

	for (int i=0; i<nbGemsOnFinish; i++)
	{
		// Initial speed for each gem. The maximum Initial speed will be 3*32 pixels/seconds.
		xSpeedOnFinish[i] = 0;
		ySpeedOnFinish[i] = -1;

	}

	return true;
}

void KD_Table::DisplayOnWin()
{
	int old_ticks = ticks;
	ticks = Display::GetTicks();
    
	character.DisplayBackground();
	DisplayBorders();
	DisplayClown(ticks-old_ticks);	
	DisplayGemsOnWin();
}

bool KD_Table::LoadCharacter(const KD_FilePath &resourceFilePath, const KD_FilePath &actionFilePath)
{
	return character.Load(resourceFilePath, actionFilePath);
}

bool KD_Table::UnloadCharacter()
{
	character.Unload();
	return true;
}

void KD_Table::TriggerCharacterAction(int actionNumber)
{
	character.TriggerCharacterAction(actionNumber);
}
