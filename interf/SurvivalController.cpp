#include <assert.h>

#include "Application.h"
#include "eventmanager.h"
#include "SurvivalController.h"
#include "../names.h"
#include "../sound/music.h"
#include "../sound/sound.h"
#include "../util/direct.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../video/textevent.h"

#define KD_A_NOACTION 0
#define KD_A_QUIT    1
#define KD_A_ADDLINE 2
#define KD_A_TAKEGEM 3
#define KD_A_DROPGEM 4
#define KD_A_REMOVEGEM 5
#define KD_A_LEFT    6
#define KD_A_RIGHT   7
#define KD_A_QUITLOSE 8


KD_SurvivalController::KD_SurvivalController() : KD_Controller()
{
  music= new KD_Music();
  plopSound= new KD_Sound();  
  
  image_manager= NULL;
  memset (images, 0, sizeof(images));

	plopSound=new KD_Sound();

	background = 0;

	currentLevel=0;
	score=0;
	clashCount=0;
	maxClashCount=0;

	comboEvent=0;

	gemsToLevel[0]=20;
	gemsToLevel[1]=50;
	gemsToLevel[2]=80;
	gemsToLevel[3]=120;
	gemsToLevel[4]=160;
	gemsToLevel[5]=200;
	gemsToLevel[6]=250;
	gemsToLevel[7]=300;
	gemsToLevel[8]=350;
	gemsToLevel[9]=400;
	gemsToLevel[10]=500;
	gemsToLevel[11]=600;
	gemsToLevel[12]=700;
	gemsToLevel[13]=800;
	gemsToLevel[14]=1000;
	
	speedOfLevel[0]=20000;
	speedOfLevel[1]=10000;
	speedOfLevel[2]=9000;
	speedOfLevel[3]=7000;
	speedOfLevel[4]=6000;
	speedOfLevel[5]=5000;
	speedOfLevel[6]=4500;
	speedOfLevel[7]=4000;
	speedOfLevel[8]=3500;
	speedOfLevel[9]=3200;
	speedOfLevel[10]=2900;
	speedOfLevel[11]=2600;
	speedOfLevel[12]=2000;
	speedOfLevel[13]=1500;
	speedOfLevel[14]=1000;
	currentTimeBetweenLines=speedOfLevel[0];
}

KD_SurvivalController::~KD_SurvivalController()
{
  delete music;
  delete plopSound;
  
  /*if (image_manager)
  {
    image_manager->releaseImage(images[0]);
  }*/
}

void KD_SurvivalController::loadSprites()
{ signed res;
  TACCRes *accFile;
  
  accFile= new TACCRes();
  res= accFile->LoadACC("art/survival.acc");
  assert(!res);
  horizontalBar= new KD_Sprite();
  res= horizontalBar->Load(accFile,"horizontalbar.txt");
  assert(res);
  verticalBar  = new KD_Sprite();
  res= verticalBar->Load(accFile,"verticalbar.txt");
  assert(res);
  upleftBar    = new KD_Sprite();
  res= upleftBar->Load(accFile,"upleftcorner.txt");
  assert(res);
  uprightBar   = new KD_Sprite();
  res= uprightBar->Load(accFile,"uprightcorner.txt");
  assert(res);

  leftDoor = new KD_Sprite();
  res= leftDoor->Load(accFile, "doorl.txt");
  assert(res);
  rightDoor = new KD_Sprite();
  res= rightDoor->Load(accFile, "doorr.txt");
  assert(res);

  bottomBar   = new KD_Sprite();
  res= bottomBar->Load(accFile, "bottombar.txt");
  assert(res);
  
  image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  image_manager->Load(accFile, "terrain2.png");
  
  background= image_manager->getImage("terrain2.png");
  background->disableAlpha();  

  res= accFile->LoadACC("art/gems.acc");
  
  short gem_index;
  for (gem_index= KD_GEM_N_RED; gem_index<= KD_GEM_N_YELLOW; gem_index++)
  { gem[gem_index]= new KD_Sprite();
    res= gem[gem_index]->Load(accFile, Gem_Anim_Filenames[gem_index]);
    /* test res ! */
  }    
  
#define TEMPO(i) { gem[i]= new KD_Sprite(); \
                   gem[i]->Load(accFile, Gem_Anim_Filenames[i]); } 
/* gemmes immondes pour tester */  
  res= accFile->LoadACC("art/gems_test.acc");
/* j'ai pas fait les normaux + symboles ni le diamant arc-en-ciel */                   
 /* TEMPO(KD_GEM_BG);
  TEMPO(KD_GEM_BC_RED);
  TEMPO(KD_GEM_B);
  TEMPO(KD_GEM_PA);
  TEMPO(KD_GEM_FL_UP);
  TEMPO(KD_GEM_FI);
  TEMPO(KD_GEM_BN_1);
  TEMPO(KD_GEM_TR);*/
   
  /* character images */
  res= accFile->LoadACC("art/charsel.acc");
                   
  characterSprite= new KD_Sprite();
  res= characterSprite->Load(accFile, CHAR_ANIM_NAME[pl_chars[0]]);
  assert(res);
                   
  clown = new KD_Sprite();
  res= accFile->LoadACC("art/chibi.acc");
  assert (!res);
  res= clown->Load(accFile, "lightchip.txt");
  clown->resize(1.8f);

  delete accFile;

  plopSound->LoadSound("art/waterdrop.wav");
}


void KD_SurvivalController::unLoadSprites()
{
/*	plopSound->UnloadSound();*/

	KD_ImageManager::getImageManager()->releaseImage(background);
	background = 0;

	delete gem[KD_GEM_N_BLUE];
	delete gem[KD_GEM_N_GREEN];
	delete gem[KD_GEM_N_RED];
	delete gem[KD_GEM_N_YELLOW];
	delete clown;
	delete uprightBar;
	delete upleftBar;
	delete verticalBar;
	delete horizontalBar;
	delete leftDoor;
	delete rightDoor;
	delete bottomBar;
	delete characterSprite;
}

void KD_SurvivalController::loadMusic(char *fileName)
{
	music->Load(fileName);
}


bool KD_SurvivalController::init()
{
	loadSprites();

	controllerState = KD_CSTATE_PLAYING;

	// Never use action 0 because it's the void action
	bindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
	bindKeyDown(SDLK_LEFT,   KD_A_LEFT);
	bindKeyDown(SDLK_RIGHT,  KD_A_RIGHT);
	bindKeyDown(SDLK_SPACE,  KD_A_ADDLINE);
	bindKeyDown(SDLK_UP,     KD_A_DROPGEM);
	bindKeyDown(SDLK_DOWN,   KD_A_TAKEGEM);

	table.setWidth(9);
	table.setHeight(12);
	table.setGemWidth(32);
	table.setGemHeight(32);
  
#define DIFFICULTY 9
signed Position_X= (640- DIFFICULTY* 32)/ 2;
	table.setPosition(Position_X,50);

	table.setHorizontalBar(horizontalBar);
	table.setVerticalBar(verticalBar);
	table.setUpperLeftBar(upleftBar);
	table.setUpperRightBar(uprightBar);
	table.setLeftDoor(leftDoor);
	table.setRightDoor(rightDoor);
	table.activateDoors(true);
	table.setBottomBar(bottomBar);

	table.setClownSprite(clown);

	table.setGems(gem);

	table.loadGemsToCome("table.txt");

	table.setLoopGems(false);

	table.setGemProbability(KD_GEM_N_BLUE, 100);
	table.setGemProbability(KD_GEM_N_RED, 100);
	table.setGemProbability(KD_GEM_N_GREEN, 100);
	table.setGemProbability(KD_GEM_N_YELLOW, 100);

	table.Init();

	loadMusic("art/survival.ogg");
	table.setPlopSound(plopSound);

	table.addLine();
	table.addLine();
	table.addLine();

	characterSpriteInstance = new KD_SpriteInstance(characterSprite);

	characterSpriteInstance->x=Position_X + DIFFICULTY*32/2;
	characterSpriteInstance->y=50 + 32*12;

	music->PlayMusic();

	timer = new KD_TextEvent();
	timer->setTextFont(Display::Slapstick);
	timer->setTextCoordinates(75,450);
	timer->printFromCenter();
	timer->activateTextTimer();
	timer->activateEvent();

	return true;
}


bool KD_SurvivalController::processEvent(int value)
{ switch(value)
	{	case KD_A_QUIT:
			KD_Application::getApplication()->sendStopEvent();
			return true;
		case KD_A_LEFT:
			table.MoveLeft();
			return true;
		case KD_A_RIGHT:
			table.MoveRight();
			return true;
		case KD_A_TAKEGEM:
			table.takeGems();
			return true;
		case KD_A_DROPGEM:
			table.dropGems();
			return true;
		case KD_A_ADDLINE:
			table.addLine();
			return true;
		case KD_A_QUITLOSE:
			//KD_Application::getApplication()->gotoController(???);
			return true;
	}

	return false;
}


bool KD_SurvivalController::display()
{
	switch (controllerState)
	{
	case KD_CSTATE_PLAYING:
		return displayPlayingState();
	case KD_CSTATE_LOSE:
		return displayLoseState();
	}
	return false;
}

bool KD_SurvivalController::displayPlayingState()
{
	/// ADD DE LIGNES TEMPORAIRE
	static int last_line_added_time=0;
	if ((signed)(SDL_GetTicks()-last_line_added_time)> currentTimeBetweenLines)
	{
		last_line_added_time = SDL_GetTicks();
		table.addLine();
	}

	background->Display(0,0);

	characterSpriteInstance->DisplayCentered();
    
	table.Display();
	Display::DisplayFramesPerSecond (12,42+2+2,20);

	if (table.getHasClashed())
	{ int i;
		for (i= 0; i<NB_LEVELS; i++)
			if (table.getNbGemsDropped() < gemsToLevel[i])
				break;

		if (currentLevel!=i)
		{
			currentLevel = i;
			currentTimeBetweenLines = speedOfLevel[i];
			KD_TextEvent *level = new KD_TextEvent();
			level->setTextFont(Display::Slapstick);
			level->printFromCenter();
			level->setText("Level %d!",currentLevel);
			level->setQuadraticMove(320,-50,255,320,150,255,320,-50,0,3);
			level->activateEvent();
		}
	}

	if (table.isTestMaxHeightNeeded())
	{
		// Test what is the maximum height of the field. If not enough, add new gems.
		int maxHeight = table.getMaxHeight();
		if (maxHeight<= 3)
        { if (table.init_tempo== 0)
			table.addLine();
          else table.init_tempo--;
        }
	
		// Test if the player has lost.
		if (maxHeight>table.getHeight())
		{
			table.prepareLose();
			timer->pauseTimer();
			controllerState = KD_CSTATE_LOSE;
			// Unbinds the keys
			bindKeyDown(SDLK_LEFT,   KD_A_NOACTION);
			bindKeyDown(SDLK_RIGHT,  KD_A_NOACTION);
			bindKeyDown(SDLK_SPACE,  KD_A_QUITLOSE);
			bindKeyDown(SDLK_UP,     KD_A_QUITLOSE);
			bindKeyDown(SDLK_DOWN,   KD_A_QUITLOSE);
		}
	}

	if (table.getHasClashed() && table.getClashCount()>1)
	{
		clashCount++;

		comboEvent = new KD_TextEvent();
		comboEvent->setTextFont(Display::Slapstick);
		comboEvent->printFromRight();
		comboEvent->setText("%d combo hits!",table.getClashCount());
		comboEvent->setQuadraticMove(640,460,255,640,380,128,640,360,0,3);
		comboEvent->activateEvent();
	}

	if (table.getClashCount() > maxClashCount && table.getClashCount()!=1) maxClashCount = table.getClashCount();

	Display::Slapstick->xycenteredprintf(565,150,"%d", clashCount);
	Display::Slapstick->xycenteredprintf(565,380,"%d", maxClashCount);
	Display::Slapstick->xycenteredprintf(70,130,"%d", table.getScore());
	Display::Slapstick->xycenteredprintf(70,290,"%d", currentLevel);

	return true;
}

bool KD_SurvivalController::displayLoseState()
{
	background->Display(0,0);

	characterSpriteInstance->DisplayCentered();

	table.DisplayOnLose();

	Display::Slapstick->xycenteredprintf(565,150,"%d", clashCount);
	Display::Slapstick->xycenteredprintf(565,380,"%d", maxClashCount);
	Display::Slapstick->xycenteredprintf(70,130,"%d", table.getScore());
	Display::Slapstick->xycenteredprintf(70,290,"%d", currentLevel);

	return true;
}

bool KD_SurvivalController::quit()
{
	delete characterSpriteInstance;

	music->StopMusic();
	music->CloseMusic();

	KD_EventManager::closeEventManager();

	unLoadSprites();

	table.deInit();
	return true;
}
