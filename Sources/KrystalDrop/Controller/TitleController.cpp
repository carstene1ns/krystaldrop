#include "../global.h"

#include "../Names.h"
#include "TitleController.h"


#include "../../KDpp/Controller/Application.h"
#include "MenuController.h"
//#include "../util/direct.h"
#include "BackgroundController.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Events/FountainEvent.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"
#include "../../KDpp/Sound/Music.h"

//#include "../../KDpp/Tools/XMLParser.h"

#define ANIM_SIZE 150
/*#define FLASHTIME 0.25
#define FLASHMUL (1.0f/FLASHTIME)*/

KD_TitleController::KD_TitleController() : KD_Controller()
{ title[0]= title[1]= NULL;
  typedef  KD_DisplayableResource* KD_DisplayablePtr;
  spr= new KD_DisplayablePtr[2];
  CHECK_ALLOC (spr);
	  
  Anim_Offset= (float*) malloc(ANIM_SIZE* sizeof(float));
  CHECK_ALLOC (Anim_Offset);
  
//  GETBACK(back);
  //back = NULL;
  //flashTime = 0;
}
  

KD_TitleController::~KD_TitleController()
{
  FREE (Anim_Offset);
  DELETE_ARR (spr);
}


void KD_TitleController::DisplayTitle()
{ float incr= (Display::GetTimeElapsed());  
  
#ifndef NO_OPENGL
  if (Display::getIsOpenGL())
  { if (state== 0)
      title[0]->DisplayColorZoomRotate (70, 140, 255, 0, 255, 168,
                          1.0f, 1.0f, 0, 0, (70- x_f)/ 400);
    if (state2== 0)
      title[1]->DisplayColorZoomRotate (320, 140, 255, 0, 255, 168,
                          1.0f, 1.0f, 0, 0, (185-y_f)/ 300);
  }
#endif
  
  if (SDL_GetTicks()- first_tick< 1000) return;
  
  // "Drop"
  if (state2== 0) { y_f+= incr* 450; /*title[1]->y*//*y_f= (int) y_f;*/ }
  if (state2== 0 && y_f> 185) { state2= 1; y_f= 0; }
  if (state2== 1)
  { y_f+= incr*120;
    if (y_f> ANIM_SIZE- 1) y_f= ANIM_SIZE- 1;
    /*title[1]->y*/y_f= (int) (185- Anim_Offset[(short int) y_f]);
  }

  // "Krystal"
  if (state== 0) { x_f+= incr* 650; /*title[0]->x= (int) x_f;*/ }
  if (state== 0 && x_f> 70) 
  { state= 1; 
    x_f= 0; 
	x_bounce = 0;

#ifndef NO_OPENGL  
    if (Display::getIsOpenGL())
    { KD_FountainEvent *fount= new KD_FountainEvent();
      fount->setCoordinates(SCR_HW,SCR_H);
      fount->setTimeToLive(11);
      //fount->setParticle(0.0f,-4.5f,20.0f/180.0f*3.14f, 0.2f, 0.02f,particle,20);
	  fount->setParticle(0.0f,-400.5f,20.0f/180.0f*3.14f, 0.2f, 90.0f,particle,20);
      fount->setParticleColors(255,255,255,255,255,0,0,160);
      fount->ActivateEvent();
	  AddEvent(fount);
    
      fount= new KD_FountainEvent();
      fount->setCoordinates(0,SCR_H);
      fount->setTimeToLive(11);
      fount->setParticle(40.0f,-200.5f,20.0f/180.0f*3.14f, 0.2f, 90.0f,particle,20);
      fount->setParticleColors(255,255,255,255,0,255,255,0);
      fount->ActivateEvent();
	  AddEvent(fount);

      fount= new KD_FountainEvent();
      fount->setCoordinates(SCR_W,SCR_H);
      fount->setTimeToLive(11);
      fount->setParticle(-40.0f,-200.5f,20.0f/180.0f*3.14f, 0.2f, 90.0f,particle,20);
      fount->setParticleColors(255,255,255,255,0,255,255,0);
      fount->ActivateEvent();
	  AddEvent(fount);
    }
#endif    
	((KD_BackgroundController*)(KD_Application::GetApplication()->GetController("Background")))->Flash();
  }
  
  if (state== 1)
  { //x_f+= incr*90;
    //if (x_f> ANIM_SIZE- 1) x_f= ANIM_SIZE- 1;
    //x_f/*title[0]->x*/= 70- Anim_Offset[(short int) x_f];
	x_bounce+= incr*90;
    if (x_bounce> ANIM_SIZE- 1) x_bounce= ANIM_SIZE- 1;
    x_f/*title[0]->x*/= 70- Anim_Offset[(short int) x_bounce];

//	  x_f = 70- Anim_Offset[ANIM_SIZE- 1];
  }

  title[0]->Display((int)x_f, 140);
  title[1]->Display(300, (int)y_f); 
}


void KD_TitleController::DisplayTexts()
{ unsigned long tick= SDL_GetTicks()- first_tick;
  
  if (tick> 9000)
    main_font->xycenteredprintf(320,340,"Press any key");
  if (tick> 2000)
  if (tick% 1500< 950) 
    main_font->xycenteredprintf(320,470, 
      "insert coin                    insert coin"); /* coin coin ! */

  if (tick> 38500)
  {
	  KD_Application::GetApplication()->EnableController ("HighScores");   
	  KD_Application::GetApplication()->DisableController(this);
  }

}


bool KD_TitleController::Init()
{ 
	
//	signed res;
//  bool b;

  /* load the graphics */
	KD_GlobalResourceSet::GetGlobalResource()->RegisterResource("big font", new KD_Font("art/Slapstick.txt"));
	big_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font");
	KD_GlobalResourceSet::GetGlobalResource()->RegisterResource("main font", big_font->resize(0.5));
	main_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("main font");
	KD_GlobalResourceSet::GetGlobalResource()->RegisterResource("medium font", big_font->resize(0.7));
	medium_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("medium font");
	KD_GlobalResourceSet::GetGlobalResource()->RegisterResource("text font", big_font->resize(0.8));
	medium_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("text font");

  /* Initialize the sprites */
  //LoadResourceFile("art/title/titleRes.txt");
  LoadArtResourceFile (this, "title", "titleRes.txt");
//background = (KD_Image *)GetResource("background");
	
//	spr = (KD_Sprite *)GetResource("cup");
  spr[0] = (KD_DisplayableResource *)GetResource("title2");
  spr[1] = (KD_DisplayableResource *)GetResource("title3");
  //b= spr[0].Load(accFile,"t_anim2.txt"); assert (b); if (b== false) return false;
  //b= spr[1].Load(accFile,"t_anim3.txt"); assert (b); if (b== false) return false; 

  
  //res= accFile->LoadACC("art/misc/star.acc");
  LoadResourceFile("art/star/star.txt");
  //particle= new KD_Sprite();
  //CHECK_ALLOC (particle);
  //res= particle->Load(accFile,"star.txt");
  particle = (KD_DisplayableResource *)GetResource("star");
  //DELETE (accFile);
  


  //PLAYMUSIC (MUSIC_NAME[KD_MUS_INTRO]);
	music = new KD_Music();
	
  
  /*particle= new KD_Sprite();
  CHECK_ALLOC (particle);
  res= particle->Load("art/misc/star.txt");*/

  
  BindKeyDown(SDLK_ESCAPE, 1);
  BindKeyDown(SDLK_SPACE, 2);
  BindKeyDown(SDLK_RETURN, 2);  

  return true;
}


bool KD_TitleController::ProcessEvent(int value)
{ switch(value)
  { case 1: KD_Application::GetApplication()->SendStopEvent();  
            return true;
    case 2: 
      ((KD_BackgroundController *)(KD_Application::GetApplication()->GetController("Background")))->Flash();
      KD_MenuController::SetMenuType (KD_MENU_GAME);
      KD_Application::GetApplication()->DisableController (this); 
      KD_Application::GetApplication()->EnableController ("Menu"); 
      return true;
  }
  
  return false;
}

bool KD_TitleController::Process()
{
	return false;
}

bool KD_TitleController::Display()
{
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif
  
  DisplayTitle();
  DisplayTexts();
  
  return true;
}


bool KD_TitleController::Quit()
{ //CLOSEMUSIC();

	delete music;

  ReleaseResource("star");
  ReleaseResource("title2");
  ReleaseResource("title3");
  
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("main font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("medium font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("text font");

  return true;
}

/*void KD_TitleController::Flash()
{
	flashTime= FLASHTIME; 
}*/

bool KD_TitleController::OnEnable()
{
	music->Load(MUSIC_NAME[KD_MUS_INTRO]);
	music->PlayMusic();

	title[0] = spr[0]->createInstance();
	title[1] = spr[1]->createInstance();
	CHECK_ALLOC (title[0]);
	CHECK_ALLOC (title[1]);

	x_f= -1100;
	y_f= -550;
	
	state= 0; state2= 0;
	//back = new KD_Background((KD_DisplayableResource *)GetResource("title1"));

	#define PER_SEC 0.5
	#define DEC 0.016
	#define AMP 60.0
	for (short index= 0; index< ANIM_SIZE; index++)
		Anim_Offset[index]=
			fabs((AMP* sin(PER_SEC* index/(2*3.14159))* exp(-DEC* index)));
  
	first_tick= SDL_GetTicks();

	return true;
}

bool KD_TitleController::OnDisable()
{
	music->StopMusic();
	music->CloseMusic();

	//DELETE (back);

	return true;
}
