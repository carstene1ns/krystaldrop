#include <assert.h>
#include <math.h>

#include "Application.h"
#include "TitleController.h"
#include "../sound/music.h"
#include "../sound/soundsystem.h"
#include "../util/direct.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/sprite.h"

#define ANIM_SIZE 150

/* title controller */
KD_TitleController::KD_TitleController(): KD_Controller()
{ title[0]= title[1]= title[2]= NULL;
  spr= new KD_Sprite[3];
  assert (spr);
  
  music= new KD_Music();
  assert (music);
  
  Anim_Offset= (float*) malloc(ANIM_SIZE* sizeof(float));
  assert (Anim_Offset);
  
  InitBackgroundXY();    
}
  

KD_TitleController::~KD_TitleController()
{ free (Anim_Offset);  
  delete music;
  delete[] spr;
}


#define X_SIZE 1280
#define Y_SIZE 960
#define X_DELTA 100
#define Y_DELTA 50
#define X_WAIT 1200
#define Y_WAIT 100
#define X_SPEED 3
#define Y_SPEED 0.8
#define BACK_FPS 50

void KD_TitleController::InitBackgroundXY()
{ for (signed index= 0; index< KD_TC_BACKGROUND_SPR/ 4; index++)
  { X_S[index   ]= (index* 4*640  / KD_TC_BACKGROUND_SPR)+ 640  ;
    X_S[index+ 5]= (index* 4*640  / KD_TC_BACKGROUND_SPR)+ 640*2;
    X_S[index+10]= (index* 4*640  / KD_TC_BACKGROUND_SPR)+ 640*2;
    X_S[index+15]= (index* 4*640  / KD_TC_BACKGROUND_SPR)+ 640  ;     
    Y_S[index   ]= (index* 4*2*480/ KD_TC_BACKGROUND_SPR)       ;
    Y_S[index+ 5]= (index* 4*2*480/ KD_TC_BACKGROUND_SPR)       ;
    Y_S[index+10]= (index* 4*2*480/ KD_TC_BACKGROUND_SPR)+ 480  ;
    Y_S[index+15]= (index* 4*2*480/ KD_TC_BACKGROUND_SPR)+ 480  ;    
  }
}


void KD_TitleController::DisplayTitle()
{ float incr= (Display::timeElapsed);
  
  if (SDL_GetTicks()- first_tick < 1000) return;
  
  /* "Drop" */
  if (state2== 0) { y_f+= incr* 450; title[2]->y= (int) y_f; }
  if (state2== 0 && y_f> 185) { state2= 1; y_f= 0; }
  if (state2== 1)
  { y_f+= incr*120;
    if (y_f> ANIM_SIZE- 1) y_f= ANIM_SIZE- 1;
    title[2]->y= (int) (185- Anim_Offset[(short int) y_f]);
  }

  /* "Krystal" */
  if (state== 0) { x_f+= incr* 650; title[1]->x= (int) x_f; }
  if (state== 0 && x_f> 70) { state= 1; x_f= 0; }
  if (state== 1)
  { x_f+= incr*90;
    if (x_f> ANIM_SIZE- 1) x_f= ANIM_SIZE- 1;
    title[1]->x= (int) (70- Anim_Offset[(short int) x_f]);
  }

  title[1]->Display();
  title[2]->Display();
}


void KD_TitleController::DisplayBackground ()
{ float incr= (Display::timeElapsed)*100;

  for (signed index= 0; index< KD_TC_BACKGROUND_SPR; index++)
  { X_S[index]-= incr* X_SPEED;
    Y_S[index]-= incr* Y_SPEED;
    if (X_S[index]< -X_SIZE/2) X_S[index]+= X_SIZE;
    if (Y_S[index]< -Y_SIZE/2) Y_S[index]+= Y_SIZE;
      
    title[0]->x= (short) X_S[index];
    title[0]->y= (short) Y_S[index];
    title[0]->Display();
  }
}


void KD_TitleController::DisplayTexts()
{ unsigned long tick= SDL_GetTicks()- first_tick;
  
  /* lots of hard coded values, that's not quite nice to see */
  if (tick> 9000)
    main_font->xyprintf(180,330,"   Press a key\n    to launch\nthe survival mode\n       demo");
  if (tick> 2000)
  if (tick% 1500<950)
  { main_font->xyprintf(10,470, "insert coin");
	main_font->xyrightprintf(630,470, "insert coin");
  }
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
  b= spr[0].Load(accFile,"t_anim1.txt"); assert (b); if (b== false) return false;
  b= spr[1].Load(accFile,"t_anim2.txt"); assert (b); if (b== false) return false; 
  b= spr[2].Load(accFile,"t_anim3.txt"); assert (b); if (b== false) return false;
  delete accFile;    

  title[0]= new KD_SpriteInstance (&spr[0]); assert (title[0]);
  title[1]= new KD_SpriteInstance (&spr[1]); assert (title[1]);
  title[2]= new KD_SpriteInstance (&spr[2]); assert (title[2]);
  title[1]->y= 140;
  title[2]->x= 300;
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
  bindKeyDown(SDLK_RETURN, 3);

  main_font= Display::Slapstick->resize(0.5);

  music->Load("art/puzzle2.ogg");
  music->PlayMusic();
  
  first_tick= SDL_GetTicks();  

  return true;
}


bool KD_TitleController::processEvent(int value)
{ switch(value)
  { case 1:  KD_Application::getApplication()->sendStopEvent(); return true;
    default: KD_Application::getApplication()->gotoController ("charsel"); return true;
  }
  
  return false;
}


bool KD_TitleController::display()
{ 
  Display::clearScreen();
  Display::DisplayFramesPerSecond (12,42+2+2,5);
  DisplayBackground();
  DisplayTitle();
  DisplayTexts();
  
  return true;
}


bool KD_TitleController::quit()
{ music->StopMusic();
  music->CloseMusic();
  
  delete main_font;
  delete title[0]; title[0]= NULL;
  delete title[1]; title[1]= NULL;
  delete title[2]; title[2]= NULL;

  return true;
}
 