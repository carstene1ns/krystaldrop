#include <assert.h>

#include "Application.h"
#include "DuelController.h"
#include "eventmanager.h"
#ifndef NO_MUSIC
#include "../sound/music.h"
#endif
#ifndef NO_SOUND
#include "../sound/sound.h"
#endif
#include "../util/direct.h"
#include "../video/font.h"
#include "../video/gem.h"
#include "../video/sprite.h"
#include "../video/Display.h"
#include "../video/spriteinstance.h"
#include "../video/textevent.h"

#define KD_A_NOACTION	0
#define KD_A_QUIT		1
#define KD_A_ADDLINE1	2
#define KD_A_TAKEGEM1	3
#define KD_A_DROPGEM1	4
#define KD_A_REMOVEGEM1	5
#define KD_A_LEFT1		6
#define KD_A_RIGHT1		7
#define KD_A_ADDLINE2	8
#define KD_A_TAKEGEM2	9
#define KD_A_DROPGEM2	10
#define KD_A_REMOVEGEM2	11
#define KD_A_LEFT2		12
#define KD_A_RIGHT2		13
#define KD_A_QUITLOSE	14

#define KD_CSTATE_READY 0
#define KD_CSTATE_PLAYING 1
#define KD_CSTATE_LOSE 2
#define KD_CSTATE_WIN 3
#define KD_CSTATE_HIGHSCORE 4

KD_DuelController::KD_DuelController(): KD_Controller()
{
#ifndef NO_MUSIC
	music= new KD_Music();
#endif  
  
#ifndef NO_SOUND  
	plopSound= new KD_Sound();
#endif  
  
	background = 0;
}

KD_DuelController::~KD_DuelController()
{
#ifndef NO_MUSIC
	delete music;
#endif  
  
#ifndef NO_SOUND  
	delete plopSound;
#endif

}

bool KD_DuelController::init()
{
	loadSprites();
	controllerState = KD_CSTATE_PLAYING;

	// Never use action 0 because it's the void action
	bindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
	bindKeyDown(SDLK_LEFT,   KD_A_LEFT2);
	bindKeyDown(SDLK_RIGHT,  KD_A_RIGHT2);
	bindKeyDown(SDLK_RETURN, KD_A_ADDLINE2);
	bindKeyDown(SDLK_RSHIFT, KD_A_DROPGEM2);
	bindKeyDown(SDLK_RCTRL,  KD_A_TAKEGEM2);
	
	bindKeyDown(SDLK_x,   KD_A_LEFT1);
	bindKeyDown(SDLK_c,  KD_A_RIGHT1);
	bindKeyDown(SDLK_v,  KD_A_ADDLINE1);
	bindKeyDown(SDLK_LSHIFT,     KD_A_DROPGEM1);
	bindKeyDown(SDLK_LCTRL,   KD_A_TAKEGEM1);

	for (int i=0; i<KD_DUEL_NB_PLAYERS; i++)
	{
		table[i].Init();

		table[i].setWidth(7);
		table[i].setHeight(12);
		table[i].setGemWidth(32);
		table[i].setGemHeight(32);

		table[i].setAllBorders(border);
		table[i].activateDoors(false);
				
		table[i].setGems(gem);

		table[i].loadGemsToCome("tableDuel.txt");

		table[i].setLoopGems(false);

        for (int gem_type= 0; gem_type< KD_GEM_NB_KINDS; gem_type++)
		  table[i].setGemProbability (gem_type, 12);

        table[i].setPosition (i==0?32:384, 50); /* ça fait plus pro :p */
		/*if (i==0)
			table[0].setPosition(32,50);
		else if (i==1)
			table[1].setPosition(384,50);*/

		table[i].InitSet();

		table[i].setClownSprite(clown[i]);


		#ifndef NO_SOUND
			table[i].setPlopSound(plopSound);
		#endif    

		table[i].addLine();
		table[i].addLine();
		table[i].addLine();
	}

	characterSpriteInstance[0] = new KD_SpriteInstance(characterSprite[0]);
	characterSpriteInstance[0]->x=32 + 7*32/2;
	characterSpriteInstance[0]->y=50 + 32*12;

	characterSpriteInstance[1] = new KD_SpriteInstance(characterSprite[1]);
	characterSpriteInstance[1]->x=384 + 7*32/2;
	characterSpriteInstance[1]->y=50 + 32*12;

	
	last_line_added_time[0]=Display::ticks;
	last_line_added_time[1]=Display::ticks;

	clashCount[0]=0;
	clashCount[1]=0;


#ifndef NO_MUSIC
	loadMusic("art/survival.ogg");
	music->PlayMusic();
#endif    

	// speed of line dropping.
	currentTimeBetweenLines = 7000;

	timeOfNewState = Display::ticks;
	return true;
}

void KD_DuelController::loadSprites()
{
	signed res;
	TACCRes *accFile;

	accFile= new TACCRes();
	res= accFile->LoadACC("art/survival.acc");
	assert(!res);
	border[KD_HORIZONTAL_BAR]= new KD_Sprite();
	res= border[KD_HORIZONTAL_BAR]->Load(accFile,"horizontalbar.txt");
	assert(res);
	border[KD_VERTICAL_BAR]  = new KD_Sprite();
	res= border[KD_VERTICAL_BAR]->Load(accFile,"verticalbar.txt");
	assert(res);
	border[KD_UPPER_LEFT_BAR]    = new KD_Sprite();
	res= border[KD_UPPER_LEFT_BAR]->Load(accFile,"upleftcorner.txt");
	assert(res);
	border[KD_UPPER_RIGHT_BAR]   = new KD_Sprite();
	res= border[KD_UPPER_RIGHT_BAR]->Load(accFile,"uprightcorner.txt");
	assert(res);

	border[KD_LEFTDOOR] = new KD_Sprite();
	res= border[KD_LEFTDOOR]->Load(accFile, "doorl.txt");
	assert(res);
	border[KD_RIGHTDOOR] = new KD_Sprite();
	res= border[KD_RIGHTDOOR]->Load(accFile, "doorr.txt");
	assert(res);

	border[KD_BOTTOM_BAR]   = new KD_Sprite();
	res= border[KD_BOTTOM_BAR]->Load(accFile, "bottombar.txt");
	assert(res);

	KD_ImageManager::getImageManager()->Load(accFile, "terrain2.png");
	background= KD_ImageManager::getImageManager()->getImage("terrain2.png");
	background->disableAlpha();  

	res= accFile->LoadACC("art/gems.acc");  
    for (short gem_index= 0; gem_index< KD_GEM_NB_KINDS; gem_index++)
	{ gem[gem_index]= new KD_Sprite();
	  res= gem[gem_index]->Load(accFile, GEM_ANIM_NAME[gem_index]);
	}
  
	/* character images */
	res= accFile->LoadACC("art/charsel.acc");

	characterSprite[0]= new KD_Sprite();
	res= characterSprite[0]->Load(accFile, /*CHAR_ANIM_NAME[pl_chars[0]]*/"forest.txt");
	assert(res);

	characterSprite[1]= new KD_Sprite();
	res= characterSprite[1]->Load(accFile, "snow.txt");
	assert(res);

	clown[0] = new KD_Sprite();
	clown[1] = new KD_Sprite();
	
	res= accFile->LoadACC("art/chibi.acc");
	assert (!res);
	res= clown[0]->Load(accFile, "lightchip.txt");
	res= clown[1]->Load(accFile, "lightchip.txt");
	
	// ARF, si on charge 2 fois le meme, on l'agrandit 2 fois!!!
	clown[0]->resize(1.8f);
	//clown[1]->resize(1.8f);

	delete accFile;

#ifndef NO_SOUND
	plopSound->LoadSound("art/waterdrop.wav");
#endif  
}


void KD_DuelController::unLoadSprites()
{
#ifndef NO_SOUND
	plopSound->UnloadSound();
#endif

	KD_ImageManager::getImageManager()->releaseImage(background);
	background = 0;

	delete border[KD_HORIZONTAL_BAR];
	delete border[KD_VERTICAL_BAR];
	delete border[KD_UPPER_LEFT_BAR];
	delete border[KD_UPPER_RIGHT_BAR];
	delete border[KD_LEFTDOOR];
	delete border[KD_RIGHTDOOR];
	delete border[KD_BOTTOM_BAR];

  for (short gem_index= 0; gem_index< KD_GEM_NB_KINDS; gem_index++)
  { if (gem[gem_index]!= NULL)
      delete gem[gem_index];
    gem[gem_index]= NULL;
  }
 	
  for (short i=0; i<KD_DUEL_NB_PLAYERS; i++)
  { delete characterSprite[i];
    delete clown[i];
  }
}


bool KD_DuelController::processEvent(int value)
{ 
	switch(value)
	{
		case KD_A_QUIT:
			KD_Application::getApplication()->sendStopEvent();
			return true;
		case KD_A_LEFT1:
			table[0].MoveLeft();
			return true;
		case KD_A_RIGHT1:
			table[0].MoveRight();
			return true;
		case KD_A_TAKEGEM1:
			table[0].takeGems();
			return true;
		case KD_A_DROPGEM1:
			table[0].dropGems();
			return true;
		case KD_A_ADDLINE1:
			table[0].addLine();
			return true;
		case KD_A_LEFT2:
			table[1].MoveLeft();
			return true;
		case KD_A_RIGHT2:
			table[1].MoveRight();
			return true;
		case KD_A_TAKEGEM2:
			table[1].takeGems();
			return true;
		case KD_A_DROPGEM2:
			table[1].dropGems();
			return true;
		case KD_A_ADDLINE2:
			table[1].addLine();
			return true;
		case KD_A_QUITLOSE:
			//KD_Application::getApplication()->gotoController(???);
			return true;
		
	}

	return false;
}

bool KD_DuelController::display()
{ 
	switch (controllerState)
	{
	case KD_CSTATE_PLAYING:
		return displayPlayingState();
	case KD_CSTATE_LOSE:
		//return displayLoseState();
	case KD_CSTATE_HIGHSCORE:
		//return displayHighScoreState();
		break;
	}
	return false;
}

bool KD_DuelController::displayPlayingState()
{
	background->Display(0,0);

	displayTable(0);
	displayTable(1);

	Display::DisplayFramesPerSecond (12,42+2+2,20);

	int timeRemaining = 90-(Display::ticks-timeOfNewState)/1000;
	Display::Slapstick->xycenteredprintf(320,200,"%d", timeRemaining);

	Display::Slapstick->xycenteredprintf(320,300,"%d", 150-table[0].getNbGemsDropped());

	Display::Slapstick->xycenteredprintf(320,400,"%d", 150-table[1].getNbGemsDropped());

	

//	Display::Slapstick->xycenteredprintf(565,150,"%d", clashCount);
//	Display::Slapstick->xycenteredprintf(565,380,"%d", maxClashCount);
//	Display::Slapstick->xycenteredprintf(70,130,"%d", table.getScore());
//	Display::Slapstick->xycenteredprintf(70,290,"%d", currentLevel);

	return true;
}

bool KD_DuelController::displayTable(short nbTable)
{
	/// ADD DE LIGNES TEMPORAIRE
	if ((signed)(SDL_GetTicks()-last_line_added_time[nbTable])> currentTimeBetweenLines)
	{
		last_line_added_time[nbTable] = SDL_GetTicks();
		table[nbTable].addLine();
	}

	characterSpriteInstance[nbTable]->Display (1);
	table[nbTable].Display();


/*	if (table[nbTable].getHasClashed())
	{ 
		/// CODE TO SEND LINES TO OPPONENT
		
	}*/

	if (table[nbTable].isTestMaxHeightNeeded())
	{
		// Test what is the maximum height of the field. If not enough, add new gems.
		int maxHeight = table[nbTable].getMaxHeight();
		if (maxHeight <= 3 && table[nbTable].isAddingGems()==false && table[nbTable].getIsHoldingGems()==false && table[nbTable].getClashCount()==0)
        { 
			table[nbTable].addLine();
			last_line_added_time[nbTable] = SDL_GetTicks();
		}
	
		// Test if the player has lost.
		if (maxHeight>table[nbTable].getHeight())
		{
			table[nbTable].prepareLose();
			//timer->pauseTimer();
			timeOfNewState = Display::ticks;
			controllerState = KD_CSTATE_LOSE;
			
			// Unbinds the keys
			bindKeyDown(SDLK_LEFT,   KD_A_NOACTION);
			bindKeyDown(SDLK_RIGHT,  KD_A_NOACTION);
			bindKeyDown(SDLK_RETURN, KD_A_QUITLOSE);
			bindKeyDown(SDLK_RSHIFT, KD_A_QUITLOSE);
			bindKeyDown(SDLK_RCTRL,  KD_A_QUITLOSE);
	
			bindKeyDown(SDLK_w,		 KD_A_NOACTION);
			bindKeyDown(SDLK_x,		 KD_A_NOACTION);
			bindKeyDown(SDLK_c,		 KD_A_QUITLOSE);
			bindKeyDown(SDLK_LSHIFT, KD_A_QUITLOSE);
			bindKeyDown(SDLK_LCTRL,  KD_A_QUITLOSE);
		}
	}

	if (table[nbTable].getHasClashed() && table[nbTable].getClashCount()>1)
	{
//		clashCount++;
		KD_TextEvent *comboEvent;
		comboEvent = new KD_TextEvent();
		comboEvent->setTextFont(Display::Slapstick);
		comboEvent->printFromRight();
		comboEvent->setText("%d combo hits!",table[nbTable].getClashCount());
		//comboEvent->setQuadraticMove(640,460,255,640,380,128,640,360,0,3);
		if (nbTable==0)
		{
			comboEvent->printFromLeft();
			comboEvent->setQuadraticMove(-320,40,255,0,40,150,20,40,0,3);
		}
		else if (nbTable==1)
		{
			comboEvent->printFromRight();
			comboEvent->setQuadraticMove(960,40,255,640,40,150,620,40,0,3);
		}
		comboEvent->activateEvent();
	}

	if (table[nbTable].getClashCountFinished() > 1)
	{
		for (int i=0; i<table[nbTable].getClashCountFinished(); i++)
			table[1-nbTable].addLine();

		last_line_added_time[1-nbTable] = SDL_GetTicks();

		KD_TextEvent *warningEvent;
		warningEvent = new KD_TextEvent();
		warningEvent->setTextFont(Display::Slapstick);
		warningEvent->setText("Warning!\n%d lines\ncoming!",table[nbTable].getClashCountFinished());
		//warningEvent->setQuadraticMove(640,460,255,640,380,128,640,360,0,3);
		if (nbTable==1)
		{
			warningEvent->printFromLeft();
			warningEvent->setQuadraticMove(20,350,255,20,350,200,20,350,0,3);
		}
		else if (nbTable==0)
		{
			warningEvent->printFromRight();
			warningEvent->setQuadraticMove(620,350,255,620,350,200,620,350,0,3);
		}
		warningEvent->setBlinking(0.2f,0.2f);
		warningEvent->activateEvent();

	}

	//if (table.getClashCount() > maxClashCount && table.getClashCount()!=1) 
	//	maxClashCount = table.getClashCount();

	return true;
}


bool KD_DuelController::quit()
{
	delete characterSpriteInstance[0];
	delete characterSpriteInstance[1];

#ifndef NO_MUSIC  
	music->StopMusic();
	music->CloseMusic();
#endif  

	KD_EventManager::getEventManager()->deleteAllEvents();

	unLoadSprites();

	table[0].deInit();
	table[0].desalloc();
	table[1].deInit();
	table[1].desalloc();
	return true;
}

#ifndef NO_MUSIC
void KD_DuelController::loadMusic(char *fileName)
{
	music->Load(fileName);
}
#endif


