#include <assert.h>
#include <math.h>

#include "Application.h"
#include "TitleController.h"
#include "../sound/music.h"
#include "../sound/soundsystem.h"
#include "../util/direct.h"
#include "../video/background.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/sprite.h"

#define ANIM_SIZE 150


/* title controller */
KD_TitleController::KD_TitleController(): KD_Controller()
{ title[0]= title[1]= NULL;
  spr= new KD_Sprite[2];
  assert (spr);
  
  music= new KD_Music();
  assert (music);
  
  Anim_Offset= (float*) malloc(ANIM_SIZE* sizeof(float));
  assert (Anim_Offset);
  
  back= KD_Background::GetBackground();
  assert (back);
}
  

KD_TitleController::~KD_TitleController()
{ free (Anim_Offset);  
  delete music;
  delete[] spr;
}


void KD_TitleController::DisplayTitle()
{ float incr= (Display::timeElapsed);
  
  if (SDL_GetTicks()- first_tick < 1000) return;
  
  /* "Drop" */
  if (state2== 0) { y_f+= incr* 450; title[1]->y= (int) y_f; }
  if (state2== 0 && y_f> 185) { state2= 1; y_f= 0; }
  if (state2== 1)
  { y_f+= incr*120;
    if (y_f> ANIM_SIZE- 1) y_f= ANIM_SIZE- 1;
    title[1]->y= (int) (185- Anim_Offset[(short int) y_f]);
  }

  /* "Krystal" */
  if (state== 0) { x_f+= incr* 650; title[0]->x= (int) x_f; }
  if (state== 0 && x_f> 70) { state= 1; x_f= 0; }
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
  
  /* lots of hard coded values, that's not quite nice to see */
  if (tick> 9000)
    main_font->xyprintf(180,330,"   Press a key\n    to launch\nthe survival mode\n       demo");
  if (tick> 2000)
  if (tick% 1500< 950)
  { main_font->xyprintf(10,470, "insert coin");
	main_font->xyrightprintf(630,470, "insert coin");
  }

  if (tick> 39000) KD_Application::getApplication()->gotoController ("highscores");   
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
  delete accFile;    

  title[0]= new KD_SpriteInstance (&spr[0]); assert (title[0]);
  title[1]= new KD_SpriteInstance (&spr[1]); assert (title[1]);
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

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 2);

  main_font= Display::Slapstick->resize(0.5);

  music->Load("art/puzzle2.ogg");
  music->PlayMusic();
  
  first_tick= SDL_GetTicks();  

  return true;
}


bool KD_TitleController::processEvent(int value)
{ switch(value)
  { case 1: KD_Application::getApplication()->sendStopEvent(); return true;
    case 2: KD_Application::getApplication()->gotoController ("charsel"); return true;
  }
  
  return false;
}


bool KD_TitleController::display()
{ 
  Display::clearScreen();
  Display::DisplayFramesPerSecond (12,42+2+2,5);
  
  assert (back);
  back->Display();
  
  DisplayTitle();
  DisplayTexts();
  
  return true;
}


bool KD_TitleController::quit()
{ music->StopMusic();
  music->CloseMusic();
  
  //delete main_font;
  delete title[0]; title[0]= NULL;
  delete title[1]; title[1]= NULL;

  return true;
}

