#include "../global.h"

#include "Application.h"
#include "DuelController.h"
#include "eventmanager.h"
#ifndef NO_SOUND
#include "../sound/sound.h"
#endif
#include "../util/direct.h"
#include "../video/font.h"
#include "../video/gem.h"
#include "../video/sprite.h"
#include "../video/Display.h"
#include "../video/spriteinstance.h"
#include "../video/fountainevent.h"
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
#define KD_CSTATE_FINISH 2
#define KD_CSTATE_WIN 3
#define KD_CSTATE_HIGHSCORE 4

KD_DuelController::KD_DuelController(): KD_Controller()
{
#ifndef NO_SOUND  
	plopSound= new KD_Sound();
#endif  
  
	background= 0;

}

KD_DuelController::~KD_DuelController()
{
#ifndef NO_SOUND  
	DELETE (plopSound);
#endif
}

bool KD_DuelController::init()
{
	loadSprites();

	bindKeyDown(SDLK_ESCAPE, KD_A_QUIT);

	nbRounds=2;
	
	cup = new KD_SpriteInstancePtr[nbRounds*KD_DUEL_NB_PLAYERS];

	controllerState = KD_CSTATE_READY;
	

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

        table[i].setPosition (i==0?32:384, 50); /* ça fait plus pro :p *//* Ouahh trop fort! ;)*/

		table[i].InitSet();
        
		table[i].setClownSprite(clown[i]);

		#ifndef NO_SOUND
			table[i].setPlopSound(plopSound);
		#endif
		
		nbWon[i]=0;

		for (int j=0; j<nbRounds; j++)
		{
			cup[j+i*nbRounds] = new KD_SpriteInstance(cupSprite);
			cup[j+i*nbRounds]->x = (i==0)? 32+7*32-(nbRounds-j)*32 : 384+j*32 ;
			cup[j+i*nbRounds]->y = 442;
		}

	}

	characterSpriteInstance[0] = new KD_SpriteInstance(characterSprite[0]);
	characterSpriteInstance[0]->x=32 + 7*32/2;
	characterSpriteInstance[0]->y=50 + 32*12;

	characterSpriteInstance[1] = new KD_SpriteInstance(characterSprite[1]);
	characterSpriteInstance[1]->x=384 + 7*32/2;
	characterSpriteInstance[1]->y=50 + 32*12;
		
	PLAYMUSIC ("art/survival.ogg");

	initReadyState();

	return true;
}

bool KD_DuelController::initRound()
{
	// Never use action 0 because it's the void action
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
		table[i].addLine();
		table[i].addLine();
		table[i].addLine();

		hasWon[i]=true;
	}

	last_line_added_time[0]=Display::ticks;
	last_line_added_time[1]=Display::ticks;

	clashCount[0]=0;
	clashCount[1]=0;

	timeOfNewState = Display::ticks;

	return true;
}

bool KD_DuelController::initReadyState()
{
	// Should empty each table.

	// speed of line dropping.
	currentTimeBetweenLines = 7000;

	timeOfNewState = Display::ticks;

	KD_TextEvent *ready = new KD_TextEvent();
	ready->setTextFont(Display::Slapstick);
	ready->printFromCenter();
	ready->setText("Ready?");
	ready->setQuadraticMove(320,-50,255,255,255,255,0.5f,0.5f,0,
							320,/*150*/240,255,255,255,/*128*/250,1.0f,1.0f,0,
							320,/*150*/240,255,255,255,0,2.0f,2.0f,0,3);
	ready->activateEvent();

	return true;
}

void KD_DuelController::loadSprites()
{	signed res;
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

	res= accFile->LoadACC("art/misc/cup.acc");
	cupSprite = new KD_Sprite();
	res= cupSprite->Load(accFile, "cup.txt");

	res= accFile->LoadACC("art/misc/star.acc");
	particle= new KD_Sprite();
	CHECK_ALLOC (particle);
	res= particle->Load(accFile,"star.txt"); 

	delete accFile;

#ifndef NO_SOUND
	plopSound->LoadSound("art/waterdrop.wav");
#endif  
}


void KD_DuelController::unLoadSprites()
{ short i;
  
#ifndef NO_SOUND
	plopSound->UnloadSound();
#endif

	KD_ImageManager::getImageManager()->releaseImage(background);
	background= 0;

	DELETE (border[KD_HORIZONTAL_BAR]);
	DELETE (border[KD_VERTICAL_BAR]);
	DELETE (border[KD_UPPER_LEFT_BAR]);
	DELETE (border[KD_UPPER_RIGHT_BAR]);
	DELETE (border[KD_LEFTDOOR]);
	DELETE (border[KD_RIGHTDOOR]);
	DELETE (border[KD_BOTTOM_BAR]);

	DELETE (cupSprite);
	DELETE (particle);

  for (i= 0; i< KD_GEM_NB_KINDS; i++) DELETE (gem[i]);
  for (i= 0; i< KD_DUEL_NB_PLAYERS; i++)
  { DELETE (characterSprite[i]);
    DELETE (clown[i]);
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
			// Pas efficace: si on appuie très vite sur le bouton faire tomber des lignes, les lignes s'empilent dans la stack et on peut quand même mourrir en s'ajoutant des lignes.
			if (table[0].getMaxHeight()<table[0].getHeight())
			{
				table[0].addLine();
				last_line_added_time[0] = SDL_GetTicks();
			}
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
			// Pas efficace: si on appuie très vite sur le bouton faire tomber des lignes, les lignes s'empilent dans la stack et on peut quand même mourrir en s'ajoutant des lignes.
			if (table[1].getMaxHeight()<table[1].getHeight())
			{
				table[1].addLine();
				last_line_added_time[1] = SDL_GetTicks();
			}
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
	case KD_CSTATE_READY:
		return displayReadyState();
	case KD_CSTATE_PLAYING:
		return displayPlayingState();
	case KD_CSTATE_FINISH:
		return displayFinishState();
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
	Display::Slapstick->xycenteredprintf (SCR_HW,200,"%d", timeRemaining);
	Display::Slapstick->xycenteredprintf (SCR_HW,300,"%d", 150-table[0].getNbGemsDropped());
	Display::Slapstick->xycenteredprintf (SCR_HW,400,"%d", 150-table[1].getNbGemsDropped());

	for (int i=0; i<nbRounds*KD_DUEL_NB_PLAYERS; i++)
        cup[i]->Display();

	// if we are going to step into win/lose screen next frame then, resolves the scores.
	if (controllerState == KD_CSTATE_FINISH)
	{
		timeRemainingWhenFinished = 90-(Display::ticks-timeOfNewState)/1000;

		timeOfNewState = Display::ticks;

		//timer->pauseTimer();

		if (hasWon[0]==false && hasWon[1]==true)
		{
			cup[nbRounds+nbWon[1]]->setAnim(1);
			nbWon[1]++;
		}
		else if (hasWon[0]==true && hasWon[1]==false)
		{
			cup[nbWon[0]]->setAnim(1);
			nbWon[0]++;
		}

		for (int i=0; i<2; i++)
		{
			if (hasWon[i]==true)
			{
				table[i].prepareWin();
				KD_TextEvent *win = new KD_TextEvent();
				win->setTextFont(Display::Slapstick);
				win->printFromCenter();
				win->setText("Victory!");
				int xPos = (i==0) ? 32 + 7*32/2 : 384 + 7*32/2;
				win->setQuadraticMove(  xPos,240,255,255,255,0  ,0.33f,0.33f, 0,
										xPos,240,255,255,255,200,0.66f,-0.66f,0,
										xPos,240,255,255,255,255,1.0f,1.0f, 0,2);
				win->activateEvent();

				KD_FountainEvent *fount= new KD_FountainEvent();
				fount= new KD_FountainEvent();
				fount->setCoordinates((i==0) ? 32 : 384,50+12*32);
				fount->setTimeToLive(2);
				fount->setParticle(0.5f,-2.5f,20.0f/180.0f*3.14f, 0.2f, 0.02f,particle,20);
				fount->setParticleColors(255,255,255,255,255,0,0,160);
				fount->activateEvent();

				fount= new KD_FountainEvent();
				fount->setCoordinates((i==0) ? 32 + 7*32 : 384 + 7*32,50+12*32);
				fount->setTimeToLive(2);
				fount->setParticle(-0.5f,-2.5f,20.0f/180.0f*3.14f, 0.2f, 0.02f,particle,20);
				fount->setParticleColors(255,255,255,255,255,0,0,160);
				fount->activateEvent();
			}
			else
			{
				table[i].prepareLose();
			}
		}
	}

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
			controllerState = KD_CSTATE_FINISH;
			
			// By default, hasWon = true, we put it to false.
			hasWon[nbTable] = false;

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

		if (nbTable==1)
		{
			warningEvent->printFromLeft();
			warningEvent->setQuadraticMove(20,80,255,20,80,200,20,80,0,3);
		}
		else if (nbTable==0)
		{
			warningEvent->printFromRight();
			warningEvent->setQuadraticMove(620,80,255,620,80,200,620,80,0,3);
		}
		warningEvent->setBlinking(0.2f,0.2f);
		warningEvent->activateEvent();

	}

	//if (table.getClashCount() > maxClashCount && table.getClashCount()!=1) 
	//	maxClashCount = table.getClashCount();

	return true;
}
 
bool KD_DuelController::displayFinishState()
{
	background->Display(0,0);

	for (int i=0; i<KD_DUEL_NB_PLAYERS; i++)
	{
		if (hasWon[i])
		{
			characterSpriteInstance[i]->Display (1);
			table[i].DisplayOnWin();

			if(Display::ticks - timeOfNewState > 2000)
			{
				// Print this only when the event is finished.
				//Display::Slapstick->xycenteredprintf ((i==0) ? 32 + 7*32/2 : 384 + 7*32/2,240,"Victory!");
				Display::Slapstick->xycoloralpharotozoomcenteredprintf(255,255,255,255, 1.5f,1.5f, (i==0) ? 32 + 7*32/2 : 384 + 7*32/2,240, -70*3.14f/180, (i==0) ? 32 + 7*32/2 : 384 + 7*32/2,240, "Victory!");
			}
		}
		else
		{
			characterSpriteInstance[i]->Display (1);
			table[i].DisplayOnLose();
			Display::Slapstick->xycoloralpharotozoomcenteredprintf(255,255,255,255, 1.5f,1.5f, (i==0) ? 32 + 7*32/2 : 384 + 7*32/2,240, -70*3.14f/180, (i==0) ? 32 + 7*32/2 : 384 + 7*32/2,240, "You lose!");
		}
	}
	
	//Display::DisplayFramesPerSecond (12,42+2+2,20);

	Display::Slapstick->xycenteredprintf (SCR_HW,200,"%d", timeRemainingWhenFinished);
	Display::Slapstick->xycenteredprintf (SCR_HW,300,"%d", 150-table[0].getNbGemsDropped());
	Display::Slapstick->xycenteredprintf (SCR_HW,400,"%d", 150-table[1].getNbGemsDropped());

	for (int i=0; i<nbRounds*KD_DUEL_NB_PLAYERS; i++)
		cup[i]->Display();

	return true;
}

bool KD_DuelController::displayReadyState()
{
	background->Display(0,0);

	for (int i=0; i<KD_DUEL_NB_PLAYERS; i++)
	{
		characterSpriteInstance[i]->Display (1);
		table[i].Display();
	}
	
	//Display::DisplayFramesPerSecond (12,42+2+2,20);

	Display::Slapstick->xycenteredprintf (SCR_HW,200,"%d", 90);
	Display::Slapstick->xycenteredprintf (SCR_HW,300,"%d", 150-table[0].getNbGemsDropped());
	Display::Slapstick->xycenteredprintf (SCR_HW,400,"%d", 150-table[1].getNbGemsDropped());

	for (int i=0; i<nbRounds*KD_DUEL_NB_PLAYERS; i++)
		cup[i]->Display();

	if (Display::ticks - timeOfNewState > 3000)
	{
		initRound();

		KD_TextEvent *goText = new KD_TextEvent();
		goText->setTextFont(Display::Slapstick);
		goText->printFromCenter();
		goText->setText("GO!");
		goText->setQuadraticMove(320,240,255,255,255,255,0.5f,0.5f,0,
								320,/*150*/240,255,255,255,/*128*/250,1.0f,1.0f,0,
								320,/*150*/240,255,255,255,0,2.0f,2.0f,0,1);
		goText->activateEvent();


		controllerState = KD_CSTATE_PLAYING;
	}

	return true;
}


bool KD_DuelController::quit()
{
	delete characterSpriteInstance[0];
	delete characterSpriteInstance[1];
	delete[] cup;

    CLOSEMUSIC();

	unLoadSprites();

	table[0].deInit();
	table[0].desalloc();
	table[1].deInit();
	table[1].desalloc();

  return KD_Controller::quit();  
}
