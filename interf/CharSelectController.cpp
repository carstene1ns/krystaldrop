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
#include "../video/image.h"
#include "../video/imagemanager.h"
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
  
  srand (SDL_GetTicks());
  sel_char= rand()% KD_NB_CHAR;
  angle= sel_char* (2* 3.14159/ KD_NB_CHAR)+ 3.14159;
}
  

KD_CharSelectController::~KD_CharSelectController()
{ if (music!= NULL)
  { delete music;
    music= NULL;
  }
}


void KD_CharSelectController::DisplayTexts()
{ font[0]->xyrightprintf (632, 50, "Character\n    Select");
}


#define KD_CSC_CENTER_X1 320
#define KD_CSC_CENTER_Y1 300
#define KD_CSC_CENTER_R1 120
#define KD_CSC_E1 1.3
#define KD_CSC_CENTER_X2 845
#define KD_CSC_CENTER_Y2 300
#define KD_CSC_CENTER_R2 900
#define KD_CSC_E2 1
#define KD_CSC_ANGLE -0.35

void KD_CharSelectController::DisplayChars()
{ short i;
  float cur_angle;
  float incr= (2* 3.14159f)/ KD_NB_CHAR;
  float wanted_angle= sel_char* incr+ 3.14159;
  float speed;
  float inc= (Display::timeElapsed)* 100;
  
  if (fabs(wanted_angle- angle)< 0.001) speed= 0; else
  if (fabs(wanted_angle- angle)< 0.03) speed= (wanted_angle- angle)< 0 ? -0.0006 : 0.0006; else
      speed= (wanted_angle- angle)* 0.03;
    
  angle+= speed* inc;
  cur_angle= angle- KD_CSC_ANGLE;
  for (i= 0; i< KD_NB_CHAR; i++)
  { img[i]->Display (cos(cur_angle)* KD_CSC_CENTER_R2* KD_CSC_E2+ KD_CSC_CENTER_X2, 
                     sin(cur_angle)* KD_CSC_CENTER_R2+ KD_CSC_CENTER_Y2);    
    cur_angle+= incr;    
  }  
  
  cur_angle= angle;
  for (i= 0; i< KD_NB_CHAR; i++)
  { img[i+ KD_NB_CHAR]->Display (cos(cur_angle)* KD_CSC_CENTER_R1* KD_CSC_E1+ KD_CSC_CENTER_X1, 
                                 sin(cur_angle)* KD_CSC_CENTER_R1+ KD_CSC_CENTER_Y1);
    cur_angle+= incr;
  }  
}



bool KD_CharSelectController::init()
{ signed res;
  bool b;
  
  /* load the graphics */
  TACCRes* acc= new TACCRes();
  assert (acc);
  if (acc== NULL) return false;
    
  res= acc->LoadACC ("art/charsel.acc");
  assert (res== 0);
  if (res< 0) return false;
   
  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  for (short i= 0; i< KD_CSC_NB_IMG; i++)
  { b= image_manager->Load (acc, CHAR_IMG_NAME[i]);
    assert (b);
    if (b== false) return false;
    img[i]= image_manager->getImage(CHAR_IMG_NAME[i]);
  }
  
  b= spr[0].Load(acc,"ar_l.txt"); assert (b); if (b== false) return false;
  spri[0]= new KD_SpriteInstance (&spr[0]); assert (spri[0]);
  spri[0]->x= (int) (KD_CSC_CENTER_X1- KD_CSC_CENTER_R1* 0.8);
  spri[0]->y= KD_CSC_CENTER_Y1+ 12;
  spri[0]->setAnim(0);

  font[0]= Display::Slapstick;
  font[1]= Display::Slapstick->resize(0.5);

  music->Load(MUSIC_NAME[KD_MUS_CHARSELECT]);
  music->PlayMusic();

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 2);
  bindKeyDown(SDLK_LEFT, 3);
  bindKeyDown(SDLK_RIGHT, 4);
  
  return true;
}


bool KD_CharSelectController::processEvent(int value)
{ switch(value)
  { case 1:  
		KD_Application::getApplication()->sendStopEvent(); 
		return true;
    case 2:
		KD_Application::getApplication()->gotoController ("survival");
		return true;
    case 3: sel_char--; return true;
    case 4: sel_char++; return true;
  }

  return false;
}


bool KD_CharSelectController::display()
{
  Display::clearScreen();

  assert (back);
  back->Display();
  DisplayChars();
  DisplayTexts();
  spri[0]->Display();
  Display::DisplayFramesPerSecond (12,42+2+2,5);  
  
  return true;
}


bool KD_CharSelectController::quit()
{ music->StopMusic();
  music->CloseMusic();
 
  if (font[1])
  { delete (font[1]);
    font[1]= NULL;
  }
  
  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  for (short i= 0; i< KD_CSC_NB_IMG; i++)
    image_manager->releaseImage (CHAR_IMG_NAME[i]);

  delete spri[0]; spri[0]= NULL;
  
  /* Player 0 has selected his character */
  pl_chars[0]= (10+ (10- sel_char)% 10)% 10;

  return true;
}
