#include "../global.h"

#include "../../KDpp/Controller/Application.h"
#include "../../KDpp/Controller/EventManager.h"
#include "HighScoresController.h"
#include "../../KDpp/Controller/UserInterface/Keyboard.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#include "SurvivalController.h"
#include "../Names.h"
#ifndef NO_SOUND
#include "../../KDpp/Sound/Sound.h"
#include "../../KDpp/Sound/Music.h"
#endif
//#include "../util/direct.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
#include "../Video/InputBox.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"
#include "../../KDpp/Video/Events/SpriteEvent.h"
#include "../../KDpp/Video/Events/TextEvent.h"
#include "../../KDpp/Video/Events/FountainEvent.h"

#define KD_A_NOACTION 0
#define KD_A_QUIT    1
#define KD_A_ADDLINE 2
#define KD_A_TAKEGEM 3
#define KD_A_DROPGEM 4
#define KD_A_REMOVEGEM 5
#define KD_A_LEFT    6
#define KD_A_RIGHT   7
#define KD_A_QUITLOSE 8


KD_SurvivalController::KD_SurvivalController() : KD_Controller(), KD_ResourceSet()
{
	/*#ifndef NO_SOUND  
	plopSound= new KD_Sound();
	chocSound= new KD_Sound();
	gemsDownSound= new KD_Sound();
	gemsUpSound= new KD_Sound();
	gemsDownSound->SetVolume(50);
	gemsUpSound->SetVolume(50);
	for (int i=0; i<KD_SND_NBCLASHSOUND; i++)
	{
		clashSound[i]=new KD_Sound();
	}
	#endif*/  
  
  //image_manager= NULL;
  memset (images, 0, sizeof(images));

  background= NULL;

	currentLevel=0;
	clashCount=0;
	maxClashCount=0;

	comboEvent=0;

	timeOfNewState = Display::getTicks();

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
	
	speedOfLevel[0]=11000;
	speedOfLevel[1]=9500;
	speedOfLevel[2]=8000;
	speedOfLevel[3]=7000;
	speedOfLevel[4]=6000;
	speedOfLevel[5]=5000;
	speedOfLevel[6]=4500;
	speedOfLevel[7]=4000;
	speedOfLevel[8]=3500;
	speedOfLevel[9]=3100;
	speedOfLevel[10]=2700;
	speedOfLevel[11]=2300;
	speedOfLevel[12]=2000;
	speedOfLevel[13]=1700;
	speedOfLevel[14]=1500;
	currentTimeBetweenLines=speedOfLevel[0];
    
    leftDoor= NULL;
}

KD_SurvivalController::~KD_SurvivalController()
{ 
	/*#ifndef NO_SOUND  
	DELETE (plopSound);
	DELETE (chocSound);
	DELETE (gemsDownSound);
	DELETE (gemsUpSound);
	for (int i=0; i<KD_SND_NBCLASHSOUND; i++)
	{
		DELETE (clashSound[i]);
	}
	#endif*/
}

void KD_SurvivalController::LoadSprites()
{ 
	
	
	/*
	signed res;
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
  background->disableAlpha();  */

	LoadResourceFile("art/survival/survival.txt");
	horizontalBar = (KD_Sprite *)GetResource("horizontalbar");
	verticalBar = (KD_Sprite *)GetResource("verticalbar");
	upleftBar = (KD_Sprite *)GetResource("upleftcorner");
	uprightBar = (KD_Sprite *)GetResource("uprightcorner");
	leftDoor = (KD_Sprite *)GetResource("doorl");
	rightDoor = (KD_Sprite *)GetResource("doorr");
	bottomBar = (KD_Sprite *)GetResource("bottombar");
	background = (KD_Image *)GetResource("terrain2");
	background->disableAlpha();

  /*res= accFile->LoadACC("art/gems.acc");
  for (short gem_index= 0; gem_index< KD_GEM_NB_KINDS; gem_index++)
  { gem[gem_index]= new KD_Sprite();
    res= gem[gem_index]->Load(accFile, GEM_ANIM_NAME[gem_index]);
    // # test res ! 
  }*/
	LoadResourceFile("art/gems/gems.txt");
    for (short gem_index= 0; gem_index< KD_GEM_NB_KINDS; gem_index++)
	{ 
		gem[gem_index] = (KD_Sprite *)GetResource(GEM_ANIM_NAME[gem_index]);
	}

  /* character images */
  /*res= accFile->LoadACC("art/charsel.acc");
  characterSprite= new KD_Sprite();
  res= characterSprite->Load(accFile, CHAR_ANIM_NAME[pl_chars[0]]);
  assert(res);*/
	//LoadResourceFile("art/charsel/charsel.txt");
	LoadResourceFile("art/characters/characters.txt");
	characterSprite = (KD_Sprite *)GetResource(CHAR_ANIM_NAME[pl_chars[0]]);
                   
  /*clown = new KD_Sprite();
  res= accFile->LoadACC("art/chibi.acc");
  assert (!res);
  res= clown->Load(accFile, "lightchip.txt");
  clown->resize(1.8f);*/
	LoadResourceFile("art/chibi/chibi.txt");
	clown = (KD_Sprite *)GetResource("lightchip");
	clown->resize(1.8f);
	
  /*particle = new KD_Sprite();
  res= accFile->LoadACC("art/misc/star.acc");
  res= particle->Load(accFile,"star.txt");
*/
	LoadResourceFile("art/star/star.txt");
	particle = (KD_Sprite *)GetResource("star");

  /*res= accFile->LoadACC("art/misc/line.acc");
  lineSprite= new KD_Sprite();
  CHECK_ALLOC (lineSprite);
  res= lineSprite->Load(accFile,"line.txt");*/

	LoadResourceFile("art/line/line.txt");
	lineSprite = (KD_Sprite *)GetResource("line");
 // delete accFile;

	main_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font"); 
}


void KD_SurvivalController::UnloadSprites()
{
/*	plopSound->UnloadSound();*/

/*  KD_ImageManager::getImageManager()->releaseImage(background);
  background= NULL;

  for (short i= 0; i< KD_GEM_NB_KINDS; i++) DELETE (gem[i]);
  
  DELETE (clown);
  DELETE (uprightBar);
  DELETE (upleftBar);
  DELETE (verticalBar);
  DELETE (horizontalBar);
  DELETE (leftDoor);
  DELETE (rightDoor);
  DELETE (bottomBar);
  DELETE (characterSprite);
  DELETE (particle);
  DELETE (lineSprite);*/
	ReleaseResource("horizontalbar");
	ReleaseResource("verticalbar");
	ReleaseResource("upleftcorner");
	ReleaseResource("uprightcorner");
	ReleaseResource("doorl");
	ReleaseResource("doorr");
	ReleaseResource("bottombar");
	ReleaseResource("terrain2");

	for (short gem_index= 0; gem_index< KD_GEM_NB_KINDS; gem_index++)
	{ 
		ReleaseResource(GEM_ANIM_NAME[gem_index]);
	}

	ReleaseResource(CHAR_ANIM_NAME[pl_chars[0]]);
	ReleaseResource("lightchip");
	ReleaseResource("star");
	ReleaseResource("line");

	KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
}


bool KD_SurvivalController::Init()
{
	
    
	music = new KD_Music();
    //PLAYMUSIC("art/survival.ogg");

	

	return true;
}


bool KD_SurvivalController::ProcessEvent(int value)
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

bool KD_SurvivalController::Process()
{
	return false;
}

bool KD_SurvivalController::Display()
{
  
	switch (controllerState)
	{
	case KD_CSTATE_PLAYING:
		return DisplayPlayingState();
	case KD_CSTATE_LOSE:
		return DisplayLoseState();
	case KD_CSTATE_HIGHSCORE:
		return DisplayHighScoreState();
	}
	return false;
}

bool KD_SurvivalController::DisplayPlayingState()
{
	/// ADD DE LIGNES TEMPORAIRE
	static int last_line_added_time=0;
	if ((signed)(SDL_GetTicks()-last_line_added_time)> currentTimeBetweenLines)
	{
		last_line_added_time = SDL_GetTicks();
		table.addLine();
	}

	background->Display(0,0);
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif     

	//characterSpriteInstance->Display (1);
#define DIFFICULTY 9
signed Position_X= (640- DIFFICULTY* 32)/ 2;
	//characterSpriteInstance->Display (Position_X + DIFFICULTY*32/2, 50 + 32*12);
	characterSpriteInstance->Display (Position_X, 50);
    
	table.Display();

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
			level->SetTextFont(main_font);
			level->PrintFromCenter();
			level->SetText("Level %d!",currentLevel);
			//level->setQuadraticMove(320,-50,255,320,150,255,320,-50,0,3);
			level->setQuadraticMove(320,-50,255,255,255,255,0.5f,0.5f,0,
								    320,/*150*/100,255,255,255,/*128*/250,1.0f,1.0f,0,
									320,/*150*/80,255,255,255,0,2.0f,2.0f,0,3);
			level->ActivateEvent();
			AddEvent(level);
		}
	}

	if (table.isTestMaxHeightNeeded())
	{
		// Test what is the maximum height of the field. If not enough, add new gems.
		int maxHeight = table.getMaxHeight();
		// it may be a good idea to insert a timer before adding those lines.
		if (maxHeight <= 2 && table.isAddingGems()==false && table.getIsHoldingGems()==false && table.getClashCount()==0)
        { 
			table.addLine();
			last_line_added_time = SDL_GetTicks();
		}

		// Test if the player has lost.
		if (maxHeight>table.getHeight())
		{
			table.prepareLose();
			timer->PauseTimer();
			timeOfNewState = Display::getTicks();
			controllerState = KD_CSTATE_LOSE;
			
			// Unbinds the keys
			BindKeyDown(SDLK_LEFT,   KD_A_NOACTION);
			BindKeyDown(SDLK_RIGHT,  KD_A_NOACTION);
			BindKeyDown(SDLK_SPACE,  KD_A_QUITLOSE);
			BindKeyDown(SDLK_UP,     KD_A_QUITLOSE);
			BindKeyDown(SDLK_DOWN,   KD_A_QUITLOSE);
		}
	}

	if (table.getHasClashed() && table.getClashCount()>1)
	{
		clashCount++;

		comboEvent = new KD_TextEvent();
		comboEvent->SetTextFont(main_font);
		comboEvent->PrintFromRight();
		comboEvent->SetText("%d combo hits!",table.getClashCount());
		comboEvent->setQuadraticMove(640,460,255,640,380,128,640,360,0,3);
		comboEvent->ActivateEvent();
		AddEvent(comboEvent);
	}

	if (table.getClashCount() > maxClashCount && table.getClashCount()!=1) 
		maxClashCount = table.getClashCount();

	main_font->xycenteredprintf(565,150,"%d", clashCount);
	main_font->xycenteredprintf(565,380,"%d", maxClashCount);
	main_font->xycenteredprintf(70,130,"%d", table.getScore());
	main_font->xycenteredprintf(70,290,"%d", currentLevel);

	return true;
}

bool KD_SurvivalController::DisplayLoseState()
{
	background->Display(0,0);
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif   

//	characterSpriteInstance->Display (1);
	#define DIFFICULTY 9
	signed Position_X= (640- DIFFICULTY* 32)/ 2;
	characterSpriteInstance->Display (Position_X + DIFFICULTY*32/2, 50 + 32*12);


	table.DisplayOnLose();

	main_font->xycenteredprintf(565,150,"%d", clashCount);
	main_font->xycenteredprintf(565,380,"%d", maxClashCount);
	main_font->xycenteredprintf(70,130,"%d", table.getScore());
	main_font->xycenteredprintf(70,290,"%d", currentLevel);


	if(Display::getTicks() - timeOfNewState > 4000)
	{
		if (KD_HighScoresController::hst[0]->IsBetterHighScore(table.getScore()))
		{
			controllerState = KD_CSTATE_HIGHSCORE;

			nameBox = new KD_InputBox();
			nameBox->SetTextFont(main_font);
			nameBox->SetTextCoordinates(250,270);
			nameBox->ActivateEvent();
			AddEvent(nameBox);
		}
		else
		{
			//KD_Application::getApplication()->gotoController("highscores");
			KD_Application::getApplication()->disableController(this);
			KD_Application::getApplication()->enableController("Background", KD_LAST_POS);
			KD_Application::getApplication()->enableController("HighScores", KD_FRONT_POS);
		}
	}

	return true;
}

bool KD_SurvivalController::DisplayHighScoreState()
{
	background->Display(0,0);
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif   

//	characterSpriteInstance->Display (1);
	#define DIFFICULTY 9
	signed Position_X= (640- DIFFICULTY* 32)/ 2;
	characterSpriteInstance->Display (Position_X + DIFFICULTY*32/2, 50 + 32*12);

	table.DisplayOnLose();

	main_font->xycenteredprintf(565,150,"%d", clashCount);
	main_font->xycenteredprintf(565,380,"%d", maxClashCount);
	main_font->xycenteredprintf(70,130,"%d", table.getScore());
	main_font->xycenteredprintf(70,290,"%d", currentLevel);
	main_font->xycenteredprintf(SCR_HW,200,"Enter your name:");

	if (nameBox->GetLength() == 3)
		main_font->xycenteredprintf(SCR_HW,340,"Press Return");
		

	if (KD_Keyboard::getKeyboard()->getLastSDLKey() == SDLK_RETURN)
	{
		char *playerName = (char *) nameBox->GetText().c_str();
		KD_HighScoresController::hst[0]->InsertHigherScore(playerName, table.getScore(), pl_chars[0]);
		//KD_Application::getApplication()->gotoController("highscores");
		KD_Application::getApplication()->disableController(this);
		KD_Application::getApplication()->enableController("Background", KD_LAST_POS);
		KD_Application::getApplication()->enableController("HighScores", KD_FRONT_POS);
	}

	return true;
}

bool KD_SurvivalController::Quit()
{
	
//    CLOSEMUSIC();
	delete music;

	

    //return KD_Controller::Quit();
	return true;
}

bool KD_SurvivalController::OnEnable()
{
	LoadSprites();

	controllerState = KD_CSTATE_PLAYING;

	// Never use action 0 because it's the void action
	BindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
	BindKeyDown(SDLK_LEFT,   KD_A_LEFT);
	BindKeyDown(SDLK_RIGHT,  KD_A_RIGHT);
	BindKeyDown(SDLK_RETURN,  KD_A_ADDLINE);
	BindKeyDown(SDLK_RSHIFT, KD_A_DROPGEM);
	BindKeyDown(SDLK_RCTRL,  KD_A_TAKEGEM);
  
	#ifndef NO_SOUND
	LoadResourceFile("art/sound/sound.txt");
	plopSound = (KD_Sound*) GetResource("clapSound");
	gemsDownSound = (KD_Sound*) GetResource("gemsDownSound");
	gemsUpSound = (KD_Sound*) GetResource("gemsUpSound");
	chocSound = (KD_Sound*) GetResource("clapSound");
	//plopSound->LoadSound("art/waterdrop.wav");
	//gemsDownSound->LoadSound("art/swing.wav");
	//gemsUpSound->LoadSound("art/swing2.wav");
	//chocSound->LoadSound("art/choc.wav");
	for (int i=0; i<KD_SND_NBCLASHSOUND; i++)
	{
		//clashSound[i]->LoadSound(CHAR_CLASHSOUND_NAME[i]);
		clashSound[i] = (KD_Sound*) GetResource(CHAR_CLASHSOUND_NAME[i]);
	}
	#endif  

	

	clashCount=0;
	maxClashCount=0;
	currentLevel=0;

	table.Init();

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
	table.setLineSprite(lineSprite);

	table.setClownSprite(clown);

	table.setGems(gem);
    for (int gem_type= 0; gem_type< KD_GEM_NB_KINDS; gem_type++)
      table.setGemProbability (gem_type, 12);
    
	table.loadGemsToCome("art/table.txt");

	table.SetLoopGems(false);

	table.InitSet();

	table.addLine();
	table.addLine();
	table.addLine();

	characterSpriteInstance = (KD_SpriteInstance *)characterSprite->createInstance();
	//characterSpriteInstance = new KD_SpriteInstance(characterSprite);

	//characterSpriteInstance->x=Position_X + DIFFICULTY*32/2;
	//characterSpriteInstance->y=50 + 32*12;

	timer= new KD_TextEvent();
    CHECK_ALLOC (timer);
	timer->SetTextFont(main_font);
	timer->setCoordinates(75,450);
	timer->PrintFromCenter();
	timer->activateTextTimer();
	timer->ActivateEvent();
	AddEvent(timer);


	#ifndef NO_SOUND
	table.setPlopSound(plopSound);
	table.setGemsDownSound(gemsDownSound);
	table.setGemsUpSound(gemsUpSound);
	table.setClashSounds(clashSound);
	#endif

	/*KD_SpriteEvent *ev = new KD_SpriteEvent();
	ev->setSprite(characterSprite);
	ev->setGravityMove(320,400,0.2f,-3.0f,0.05f,255,255,255,255,255,0,0,128,3);
	ev->ActivateEvent();*/

/*	KD_FountainEvent *fount = new KD_FountainEvent();
	fount->setCoordinates(320,400);
	fount->setTimeToLive(10);
	fount->setParticle(0.0f,-6.0f,20.0f/180.0f*3.14f, 0.2f, 0.05f,particle,20);
	fount->setParticleColors(255,255,255,255,255,0,0,128);
	fount->ActivateEvent();*/


	music->Load("art/survival.ogg");
	music->PlayMusic();

	

	return true;
}

bool KD_SurvivalController::OnDisable()
{
	music->StopMusic();
	music->CloseMusic();

	DELETE (characterSpriteInstance);
    DELETE (timer); 

	UnloadSprites();

	#ifndef NO_SOUND
	ReleaseResource("clapSound");
	ReleaseResource("gemsDownSound");
	ReleaseResource("gemsUpSound");
	ReleaseResource("clapSound");
	for (int i=0; i<KD_SND_NBCLASHSOUND; i++)
	{
		ReleaseResource(CHAR_CLASHSOUND_NAME[i]);
	}
	#endif  



	table.deInit();
	table.desalloc();

	return true;
}
