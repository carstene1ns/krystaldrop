#include <assert.h>
#include <math.h>

#include "Application.h"
#include "CharSelectController.h"
#include "../sound/music.h"
#include "../sound/soundsystem.h"
#include "../util/direct.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/sprite.h"

#define ANIM_SIZE 150

/* character select controller */
KD_CharSelectController::KD_CharSelectController(): KD_Controller()
{ unsigned i;

  for (i= 0; i< KD_CSC_NB_SPRI; i++)
  { spri[i]= NULL;
  }

  for (i= 0; i< KD_CSC_NB_FONT; i++)
  { font[i]= NULL;
  }
  
  music= new KD_Music();
  assert (music);
  
  first_tick= 0; 
  first_tick= SDL_GetTicks();
  
  KD_TitleController* TC= (KD_TitleController*) 
    KD_Application::getApplication()->getController ("title");
  assert (TC);
  X_S= TC->X_S; assert (X_S);
  Y_S= TC->Y_S; assert (Y_S);
}
  

KD_CharSelectController::~KD_CharSelectController()
{ if (music!= NULL)
  { delete music;  
    music= NULL;
  }
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


void KD_CharSelectController::DisplayBackground ()
{ float incr= (Display::timeElapsed)*100;

  for (signed index= 0; index< KD_TC_BACKGROUND_SPR; index++)
  { X_S[index]-= incr* X_SPEED;
    Y_S[index]-= incr* Y_SPEED;
    if (X_S[index]< -X_SIZE/2) X_S[index]+= X_SIZE;
    if (Y_S[index]< -Y_SIZE/2) Y_S[index]+= Y_SIZE;
      
    spri[0]->x= (short) X_S[index];
    spri[0]->y= (short) Y_S[index];
    spri[0]->Display();
  }
}


void KD_CharSelectController::DisplayTexts()
{ unsigned long tick= SDL_GetTicks()- first_tick;
  
  if (tick% 1500<950)
  { font[0]->xyprintf(10,470, "insert coin");
	font[0]->xyrightprintf(630,470, "insert coin");
  }
}



bool KD_CharSelectController::init()
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
/*  b= spr[1].Load(accFile,"t_anim2.txt"); assert (b); if (b== false) return false; 
  b= spr[2].Load(accFile,"t_anim3.txt"); assert (b); if (b== false) return false;*/
  delete accFile;    
  

  spri[0]= new KD_SpriteInstance (&spr[0]); assert (spri[0]);
  /*title[1]= new KD_SpriteInstance (&spr[1]); assert (title[1]);
  title[2]= new KD_SpriteInstance (&spr[2]); assert (title[2]);
  title[1]->y= 140;
  title[2]->x= 300;*/

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 3);

  font[0]= Display::Slapstick->resize(0.5);

 /* music->Load("art/puzzle2.ogg");
  music->PlayMusic();*/

  return true;
}


bool KD_CharSelectController::processEvent(int value)
{ switch(value)
  { case 1:  KD_Application::getApplication()->sendStopEvent(); return true;
    default: KD_Application::getApplication()->gotoController ("survival"); return true;
  }

  return false;
}


bool KD_CharSelectController::display()
{
  Display::clearScreen();
  Display::DisplayFramesPerSecond (12,42+2+2,5);
  DisplayBackground();
  DisplayTexts();
  
  return true;
}


bool KD_CharSelectController::quit()
{
  music->StopMusic();
  music->CloseMusic();
  
  delete font[0];
  delete spri[0]; spri[0]= NULL;

  return true;
}
 