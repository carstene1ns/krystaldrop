#include "../global.h"

#include "Application.h"
#include "TitleController.h"
#include "MenuController.h"
#include "../util/direct.h"
#include "../video/background.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/fountainevent.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"

#define ANIM_SIZE 150

KD_TitleController::KD_TitleController(): KD_Controller()
{ title[0]= title[1]= NULL;
  spr= new KD_Sprite[2];
  CHECK_ALLOC (spr);
  
  Anim_Offset= (float*) malloc(ANIM_SIZE* sizeof(float));
  CHECK_ALLOC (Anim_Offset);
  
  GETBACK(back);
}
  

KD_TitleController::~KD_TitleController()
{
  FREE (Anim_Offset);
  DELETE_ARR (spr);
}


void KD_TitleController::DisplayTitle()
{ float incr= (Display::timeElapsed);
  
#ifndef NO_OPENGL
  if (Display::isOpenGL)
  { if (state== 0)
      title[0]->Display (70, 140, KD_SPRITE_TOP_LEFT, 255, 0, 255, 168,
                          1.0f, 1.0f, 0, 0, (70- x_f)/ 400);
    if (state2== 0)
      title[1]->Display (320, 140, KD_SPRITE_TOP_LEFT,255, 0, 255, 168,
                          1.0f, 1.0f, 0, 0, (185-y_f)/ 300);
  }
#endif
  
  if (SDL_GetTicks()- first_tick< 1000) return;
  
  // "Drop"
  if (state2== 0) { y_f+= incr* 450; title[1]->y= (int) y_f; }
  if (state2== 0 && y_f> 185) { state2= 1; y_f= 0; }
  if (state2== 1)
  { y_f+= incr*120;
    if (y_f> ANIM_SIZE- 1) y_f= ANIM_SIZE- 1;
    title[1]->y= (int) (185- Anim_Offset[(short int) y_f]);
  }

  // "Krystal"
  if (state== 0) { x_f+= incr* 650; title[0]->x= (int) x_f; }
  if (state== 0 && x_f> 70) 
  { state= 1; 
    x_f= 0; 

#ifndef NO_OPENGL  
    if (Display::isOpenGL)
    { KD_FountainEvent *fount= new KD_FountainEvent();
      fount->setCoordinates(SCR_HW,SCR_H);
      fount->setTimeToLive(11);
      fount->setParticle(0.0f,-4.5f,20.0f/180.0f*3.14f, 0.2f, 0.02f,particle,20);
      fount->setParticleColors(255,255,255,255,255,0,0,160);
      fount->activateEvent();
    
      fount= new KD_FountainEvent();
      fount->setCoordinates(0,SCR_H);
      fount->setTimeToLive(11);
      fount->setParticle(0.5f,-2.5f,20.0f/180.0f*3.14f, 0.2f, 0.02f,particle,20);
      fount->setParticleColors(255,255,255,255,0,255,255,0);
      fount->activateEvent();

      fount= new KD_FountainEvent();
      fount->setCoordinates(SCR_W,SCR_H);
      fount->setTimeToLive(11);
      fount->setParticle(-0.5f,-2.5f,20.0f/180.0f*3.14f, 0.2f, 0.02f,particle,20);
      fount->setParticleColors(255,255,255,255,0,255,255,0);
      fount->activateEvent();
    }
#endif    
    
    Display::Flash();
  }
  
  if (state== 1)
  { x_f+= incr*90;
    if (x_f> ANIM_SIZE- 1) x_f= ANIM_SIZE- 1;
    title[0]->x= (int) (70- Anim_Offset[(short int) x_f]);
  }
  
  title[0]->Display();
  title[1]->Display(); 
}


void KD_TitleController::DisplayTexts()
{ unsigned long tick= SDL_GetTicks()- first_tick;
  
  if (tick> 9000)
    main_font->xycenteredprintf(320,340,"Press any key");
  if (tick> 2000)
  if (tick% 1500< 950) 
    main_font->xycenteredprintf(320,470, 
      "insert coin                    insert coin"); /* coin coin ! */

  if (tick> 38500) KD_Application::getApplication()->gotoController ("highscores");   
}


bool KD_TitleController::init()
{ signed res;
  bool b;

  /* load the graphics */
  TACCRes* accFile= new TACCRes();
  assert (accFile);
  if (accFile== NULL) return false;
  
  /* initialize the sprites */
  res= accFile->LoadACC("art/title.acc");
  assert (res== 0);
  if (res!= 0) return false;
  b= spr[0].Load(accFile,"t_anim2.txt"); assert (b); if (b== false) return false;
  b= spr[1].Load(accFile,"t_anim3.txt"); assert (b); if (b== false) return false; 
  DELETE (accFile);

  title[0]= new KD_SpriteInstance (&spr[0]); CHECK_ALLOC (title[0]);
  title[1]= new KD_SpriteInstance (&spr[1]); CHECK_ALLOC (title[1]);
  title[0]->y= 140;
  title[1]->x= 300;
  x_f= -1100;
  y_f= -550;
  state= 0; state2= 0;

#define PER_SEC 0.5
#define DEC 0.016
#define AMP 60.0
  for (short index= 0; index< ANIM_SIZE; index++)
    Anim_Offset[index]=
      fabs((AMP* sin(PER_SEC* index/(2*3.14159))* exp(-DEC* index)));

  main_font= Display::Slapstick->resize(0.5);

  PLAYMUSIC (MUSIC_NAME[KD_MUS_INTRO]);
  
  particle= new KD_Sprite();
  CHECK_ALLOC (particle);
  res= particle->Load("art/misc/star.txt"); 
  
  first_tick= SDL_GetTicks();
  
  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2);
  bindKeyDown(SDLK_RETURN, 2);  

  return true;
}


bool KD_TitleController::processEvent(int value)
{ switch(value)
  { case 1: KD_Application::getApplication()->sendStopEvent();  
            return true;
    case 2: Display::Flash();
            KD_MenuController::SetMenuType (KD_MENU_GAME);
            KD_Application::getApplication()->gotoController ("menu"); 
            return true;
  }
  
  return false;
}


bool KD_TitleController::display()
{ Display::clearScreen();

  assert (back);
  back->Display();
  
  DisplayTitle();
  DisplayTexts();
  
  return true;
}


bool KD_TitleController::quit()
{ CLOSEMUSIC();
  DELETE (main_font);
  DELETE (title[0]);
  DELETE (title[1]);
  DELETE (particle);
  
  return KD_Controller::quit();
}

