#include <assert.h>
#include <math.h>

#include "Application.h"
#include "CharSelectController.h"
#include "../sound/music.h"
#include "../sound/soundsystem.h"
#include "../util/direct.h"
#include "../video/background.h"
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
  
  back= KD_Background::GetBackground();
  assert (back);
}
  

KD_CharSelectController::~KD_CharSelectController()
{ if (music!= NULL)
  { delete music;  
    music= NULL;
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
  

  spri[0]= new KD_SpriteInstance (&spr[0]); assert (spri[0]);

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
  assert (back);
  back->Display();
  DisplayTexts();
  
  return true;
}


bool KD_CharSelectController::quit()
{
  music->StopMusic();
  music->CloseMusic();
  
  delete font[0];
  delete spri[0]; spri[0]= NULL;
  delete back;

  return true;
}
 