#include "../global.h"

#include "Application.h"
#include "CharSelectController.h"
#include "eventmanager.h"
#include "../util/direct.h"
#include "../video/animtextevent.h"
#include "../video/background.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/image.h"
#include "../video/imagemanager.h"
#include "../video/sprite.h"

#define ANIM_SIZE 150

#define GET_SEL_CHAR(sel_char) ((10+ (10- (sel_char))% 10)% 10)

/* character select controller */
KD_CharSelectController::KD_CharSelectController(): KD_Controller()
{ unsigned i;

  for (i= 0; i< KD_CSC_NB_SPRI; i++) spri[i]= NULL;
  for (i= 0; i< KD_CSC_NB_FONT; i++) font[i]= NULL;
  
  first_tick= SDL_GetTicks();
  
  GETBACK (back);
  
  srand (SDL_GetTicks());
  sel_char= rand()% KD_NB_CHAR;
  angle= sel_char* (2* 3.14159/ KD_NB_CHAR)+ 3.14159;
  
  Title= NULL;
  Name1= NULL;
  
  spr= new KD_Sprite;
  CHECK_ALLOC (spr);
}
  

KD_CharSelectController::~KD_CharSelectController()
{ 
  DELETE (spr); 
}


#define KD_CSC_CENTER_X1 380
#define KD_CSC_CENTER_Y1 300
#define KD_CSC_CENTER_R1 120
#define KD_CSC_E1 1.3
#define KD_CSC_CENTER_X2 845
#define KD_CSC_CENTER_Y2 350
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

  res= acc->LoadACC ("art/menu.acc");
  assert (res== 0);
  if (res< 0) return false;    
  
  b= spr[0].Load(acc,"ar_l.txt"); assert (b); if (b== false) return false;
  spri[0]= new KD_SpriteInstance (&spr[0]); assert (spri[0]);
  spri[0]->x= (int) (KD_CSC_CENTER_X1- KD_CSC_CENTER_R1* 0.8);
  spri[0]->y= KD_CSC_CENTER_Y1+ 12;
  spri[0]->setAnim(0);
  
  DELETE (acc);

  font[0]= Display::Slapstick;
  font[1]= Display::Slapstick->resize(0.5);

  PLAYMUSIC (MUSIC_NAME[KD_MUS_CHARSELECT]);

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 2);
  bindKeyDown(SDLK_LEFT, 3);
  bindKeyDown(SDLK_RIGHT, 4);
  
  Title= new KD_BouncingText ("Character select", font[0], 320, 90);
  CHECK_ALLOC (Title);
  Title->activateEvent();
  
  Name1= new KD_MessageText (CHAR_NAME[GET_SEL_CHAR(sel_char)], font[1], 138, 120);
  CHECK_ALLOC (Name1);
  Name1->activateEvent();
  
  Display::flip();
  Display::flip(); /* init() takes time, the double flip is to be sure 
                      the flash is visible, by resetting timeElapsed */
  Display::Flash();
  return true;
}


bool KD_CharSelectController::processEvent(int value)
{ switch (value)
  { case 1: KD_Application::getApplication()->sendStopEvent();
            return true;
    case 2: KD_Application::getApplication()->gotoController ("survival");
            return true;
    case 3: sel_char--; 
            Name1->RemoveText(); 
            Name1= new KD_MessageText(CHAR_NAME[GET_SEL_CHAR(sel_char)], 
                                      font[1], 138, 120); 
            Name1->activateEvent();
            return true;
    case 4: sel_char++; 
            Name1->RemoveText(); 
            Name1= new KD_MessageText(CHAR_NAME[GET_SEL_CHAR(sel_char)], 
                                      font[1], 138, 120);
            Name1->activateEvent();    
            return true;
  }

  return false;
}


bool KD_CharSelectController::display()
{
  Display::clearScreen();

  assert (back);
  back->Display();

#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif    
  
  DisplayChars();
  Title->Display();
  Name1->Display();
  spri[0]->Display();
  
  return true;
}


bool KD_CharSelectController::quit()
{ 
  DELETE (Title);
  DELETE (font[1]);
  DELETE (spri[0]);  
  CLOSEMUSIC();
  
  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  for (short i= 0; i< KD_CSC_NB_IMG; i++)
    image_manager->releaseImage (CHAR_IMG_NAME[i]);
  
  /* Player 0 has selected his character */
  pl_chars[0]= GET_SEL_CHAR (sel_char);

  return KD_Controller::quit();  
}
