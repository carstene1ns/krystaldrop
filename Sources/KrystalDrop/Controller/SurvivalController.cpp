#include "../global.h"
#include "HighScoresController.h"
#include "SurvivalController.h"
#include "../Controller/KDApplication.h"
#include "../Game/ControlsConfig.h"
#include "../Names.h"
#include "../Video/InputBox.h"
#include "../../KDpp/Controller/EventManager.h"
#include "../../KDpp/Controller/UserInterface/Keyboard.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#ifndef NO_SOUND
#include "../../KDpp/Sound/Sound.h"
#endif
#ifndef NO_MUSIC
#include "../../KDpp/Sound/Music.h"
#endif
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
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
	memset (images, 0, sizeof(images));

	background= NULL;

	currentLevel=0;
	clashCount=0;
	maxClashCount=0;

	comboEvent=0;

	timeOfNewState = Display::GetTicks();

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
}

void KD_SurvivalController::LoadSprites()
{ 
	LoadResourceFile(KD_KDApplication::GetArtFile("survival.acc/survival.txt"));
	horizontalBar = (KD_Sprite *)GetResource("horizontalbar");
	verticalBar = (KD_Sprite *)GetResource("verticalbar");
	upleftBar = (KD_Sprite *)GetResource("upleftcorner");
	uprightBar = (KD_Sprite *)GetResource("uprightcorner");
	leftDoor = (KD_Sprite *)GetResource("doorl");
	rightDoor = (KD_Sprite *)GetResource("doorr");
	bottomBar = (KD_Sprite *)GetResource("bottombar");
	background = (KD_Image *)GetResource("terrain2");
	background->DisableAlpha();

	LoadResourceFile(KD_KDApplication::GetArtFile("gems.acc/gems.txt"));
	for (short gem_index= 0; gem_index< KD_GEM_NB_KINDS; gem_index++)
		gem[gem_index] = (KD_Sprite *)GetResource(GEM_ANIM_NAME[gem_index]);

  /* character images */
	string res = KD_KDApplication::GetArtDirectory()+ "characters/";
	res += CHAR_ANIM_NAME[pl_chars[0]];
	res += "/";
	res += CHAR_ANIM_NAME[pl_chars[0]];
	res += ".txt";
	string res2 = KD_KDApplication::GetArtDirectory()+ "characters/";
	res2 += CHAR_ANIM_NAME[pl_chars[0]];
	res2 += "/actions.xml";
	table.LoadCharacter(res, res2);

	LoadResourceFile(KD_KDApplication::GetArtFile("star.acc/star.txt"));
	particle = (KD_Sprite *)GetResource("star");

	LoadResourceFile(KD_KDApplication::GetArtFile("line.acc/line.txt"));
	lineSprite = (KD_Sprite *)GetResource("line");

	main_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font"); 
}


void KD_SurvivalController::UnloadSprites()
{
/*	plopSound->UnloadSound();*/

/*  DELETE (clown);
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

	table.UnloadCharacter();
	
	//ReleaseResource(CHAR_ANIM_NAME[pl_chars[0]]);
	//ReleaseResource("lightchip");
	ReleaseResource("star");
	ReleaseResource("line");

	KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
}


bool KD_SurvivalController::Init()
{
#ifndef NO_MUSIC
	music = new KD_Music();
#endif

	return true;
}


bool KD_SurvivalController::ProcessEvent(int value)
{ switch(value)
	{	case KD_A_QUIT:
			KD_Application::GetApplication()->SendStopEvent();
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
			//KD_Application::GetApplication()->gotoController(???);
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
	//if ((signed)(SDL_GetTicks()-last_line_added_time)> currentTimeBetweenLines)
	if ((signed)(Display::GetTicks()-last_line_added_time)> currentTimeBetweenLines)
	{
		//last_line_added_time = SDL_GetTicks();
		last_line_added_time = Display::GetTicks();
		table.addLine();
	}

	background->Display(0,0);
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif     

	//characterSpriteInstance->Display (1);
#define DIFFICULTY 9
//signed Position_X= (640- DIFFICULTY* 32)/ 2;
	//characterSpriteInstance->Display (Position_X + DIFFICULTY*32/2, 50 + 32*12);
	//characterSpriteInstance->Display (Position_X, 50);
    
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
			//last_line_added_time = SDL_GetTicks();
			last_line_added_time = Display::GetTicks();
		}

		// If 3/4 of the screen is filled
		if ( maxHeight*4 > table.getHeight()*3  &&  characterMood != KD_STRESSED)
		{
			characterMood = KD_STRESSED;
			table.TriggerCharacterAction(KD_DANGER);
		}
		else if ( maxHeight*2 > table.getHeight() && maxHeight*4 < table.getHeight()*3  &&  characterMood != KD_MEDIUMMOOD)
		{
			characterMood = KD_MEDIUMMOOD;
		}
		else if ( maxHeight*2 < table.getHeight()  &&  characterMood != KD_GOODMOOD)
		{
			characterMood = KD_GOODMOOD;
		}

		// Test if the player has lost.
		if (maxHeight>table.getHeight())
		{
			table.prepareLose();
			timer->PauseTimer();
			timeOfNewState = Display::GetTicks();
			controllerState = KD_CSTATE_LOSE;
			
			KD_ControlsConfig *config = KD_ControlsConfig::GetSingleton();
			assert (config);

			BindInput (config->GetControlKind(KD_ControlsConfig::p1up) ,   config->GetControlCode(KD_ControlsConfig::p1up), KD_A_QUITLOSE);
			BindInput (config->GetControlKind(KD_ControlsConfig::p1down) ,   config->GetControlCode(KD_ControlsConfig::p1down), KD_A_QUITLOSE);
			BindInput (config->GetControlKind(KD_ControlsConfig::p1left) ,   config->GetControlCode(KD_ControlsConfig::p1left), KD_A_NOACTION);
			BindInput (config->GetControlKind(KD_ControlsConfig::p1right) ,   config->GetControlCode(KD_ControlsConfig::p1right), KD_A_NOACTION);
			BindInput (config->GetControlKind(KD_ControlsConfig::p1extra) ,   config->GetControlCode(KD_ControlsConfig::p1extra), KD_A_QUITLOSE);


			table.TriggerCharacterAction(KD_LOOSING);
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

		// First Combo: this is an ATTACK
		if ( table.getClashCount() == 2 )
			table.TriggerCharacterAction(KD_ATTACK);
		// 4 Hit combo, this is a Strong Attack
		else if ( table.getClashCount() == 4 )
			table.TriggerCharacterAction(KD_STRONGATTACK);
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
//	signed Position_X= (640- DIFFICULTY* 32)/ 2;
	//characterSpriteInstance->Display (Position_X + DIFFICULTY*32/2, 50 + 32*12);


	table.DisplayOnLose();

	main_font->xycenteredprintf(565,150,"%d", clashCount);
	main_font->xycenteredprintf(565,380,"%d", maxClashCount);
	main_font->xycenteredprintf(70,130,"%d", table.getScore());
	main_font->xycenteredprintf(70,290,"%d", currentLevel);


	if(Display::GetTicks() - timeOfNewState > 4000)
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
			//KD_Application::GetApplication()->gotoController("highscores");
			KD_Application::GetApplication()->DisableController(this);
			KD_Application::GetApplication()->EnableController("Background", KD_LAST_POS);
			KD_Application::GetApplication()->EnableController("HighScores", KD_FRONT_POS);
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
//	signed Position_X= (640- DIFFICULTY* 32)/ 2;
	//characterSpriteInstance->Display (Position_X + DIFFICULTY*32/2, 50 + 32*12);

	table.DisplayOnLose();

	main_font->xycenteredprintf(565,150,"%d", clashCount);
	main_font->xycenteredprintf(565,380,"%d", maxClashCount);
	main_font->xycenteredprintf(70,130,"%d", table.getScore());
	main_font->xycenteredprintf(70,290,"%d", currentLevel);
	main_font->xycenteredprintf(SCR_HW,200,"Enter your name:");

	if (nameBox->GetLength() == 3)
		main_font->xycenteredprintf(SCR_HW,340,"Press Return");
		

	if (KD_Keyboard::GetKeyboard()->GetLastSDLKey() == SDLK_RETURN)
	{
		char *playerName = (char *) nameBox->GetText().c_str();
		KD_HighScoresController::hst[0]->InsertHigherScore(playerName, table.getScore(), pl_chars[0]);
		KD_Application::GetApplication()->DisableController(this);
		KD_Application::GetApplication()->EnableController("Background", KD_LAST_POS);
		KD_Application::GetApplication()->EnableController("HighScores", KD_FRONT_POS);
	}

	return true;
}

bool KD_SurvivalController::Quit()
{
#ifndef NO_MUSIC
	delete music;
#endif
    //return KD_Controller::Quit();
	return true;
}

bool KD_SurvivalController::OnEnable()
{
	LoadSprites();

	controllerState = KD_CSTATE_PLAYING;
  
	KD_ControlsConfig *config = KD_ControlsConfig::GetSingleton();
	assert (config);

	BindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
  /* Player will use Player 2 keys */
	BindInput (config->GetControlKind(KD_ControlsConfig::p1up) ,   config->GetControlCode(KD_ControlsConfig::p1up),   KD_A_DROPGEM);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1down),  config->GetControlCode(KD_ControlsConfig::p1down), KD_A_TAKEGEM);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1left),  config->GetControlCode(KD_ControlsConfig::p1left), KD_A_LEFT);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1right), config->GetControlCode(KD_ControlsConfig::p1right),KD_A_RIGHT);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1extra), config->GetControlCode(KD_ControlsConfig::p1extra),KD_A_ADDLINE);


#ifndef NO_SOUND
	//LoadResourceFile(KD_KDApplication::GetArtFile("sound/sound.txt"));
	LoadResourceFile(KD_KDApplication::GetArtFile("sound.acc/sound.txt"));
	plopSound = (KD_Sound*) GetResource("clapSound");
	gemsDownSound = (KD_Sound*) GetResource("gemsDownSound");
	gemsUpSound = (KD_Sound*) GetResource("gemsUpSound");
	chocSound = (KD_Sound*) GetResource("clapSound");
	for (int i=0; i<KD_SND_NBCLASHSOUND; i++)
		clashSound[i] = (KD_Sound*) GetResource(CHAR_CLASHSOUND_NAME[i]);
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

	table.setGems(gem);
    for (int gem_type= 0; gem_type< KD_GEM_NB_KINDS; gem_type++)
      table.setGemProbability (gem_type, 12);
    
	table.loadGemsToCome(KD_KDApplication::GetArtFile("table.txt").c_str());

	table.SetLoopGems(false);

	table.InitSet();

	table.addLine();
	table.addLine();
	table.addLine();

	//	characterSpriteInstance = (KD_SpriteInstance *)characterSprite->createInstance();

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

#ifndef NO_MUSIC
	music->Load(KD_KDApplication::GetArtFile(MUSIC_NAME[KD_MUS_SURVIVAL]).c_str());
	music->PlayMusic();
#endif

	return true;
}

bool KD_SurvivalController::OnDisable()
{
#ifndef NO_MUSIC
	music->StopMusic();
	music->CloseMusic();
#endif

	//DELETE (characterSpriteInstance);
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
