#include "../global.h"

#include "Application.h"
#include "eventmanager.h"
#include "MenuController.h"
#include "../names.h"
#include "../util/direct.h"
#include "../video/animtextevent.h"
#include "../video/background.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../video/imagemanager.h"

static char* DESCRIBE_TEXT[]= 
  { "Solo game",
    "2 players vs" };
    
short KD_MenuController::menu_type= KD_MENU_GAME;

KD_MenuController::KD_MenuController(): KD_Controller()
{ GETBACK(back);
}

KD_MenuController::~KD_MenuController()
{ }


bool KD_MenuController::init()
{ bool b;
  signed res;
  
  NEW (ar_r, KD_Sprite);

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 2);
  bindKeyDown(SDLK_UP, 3);
  bindKeyDown(SDLK_DOWN, 4);

  main_font= Display::Slapstick;
  text_font= Display::Slapstick->resize(0.8);
  mini_font= Display::Slapstick->resize(0.5);
  assert (main_font);
  CHECK_ALLOC (text_font);
  CHECK_ALLOC (mini_font);

  PLAYMUSIC(MUSIC_NAME[KD_MUS_INTRO]);
  
  if (menu_type== KD_MENU_GAME)
    NEW (Title, KD_BouncingText ("Game select", main_font, SCR_HW, 90));
  
  Title->activateEvent();
  
  TACCRes* acc= new TACCRes();
  assert (acc);
  if (acc== NULL) return false;
  
  res= acc->LoadACC ("art/menu.acc");
  assert (res== 0);
  if (res< 0) return false;
  
  b= ar_r[0].Load(acc,"ar_r.txt"); assert (b); if (b== false) return false;
  NEW (ar_ri, KD_SpriteInstance (ar_r));
  ar_ri->x= 120;
  ar_ri->y= 190;
  ar_ri->setAnim(0);
  
  DELETE (acc);
    
  Description= NULL;
  pos= 0;
  UpdateDescription();
  
  DELETE (acc); 
  
  return true;
}


bool KD_MenuController::processEvent(int value)
{ switch(value)
  { case 1: KD_Application::getApplication()->sendStopEvent(); return true;
    case 2: if (menu_type== KD_MENU_GAME) 
            { switch (pos)
              { case 0: KD_Application::getApplication()->gotoController ("charsel"); break;
                case 1: KD_Application::getApplication()->gotoController ("charsel2"); break;
              }
              return true;
            } else assert (0);
    case 3: /* up */
            if (menu_type== KD_MENU_GAME && pos> 0)
            { pos--;
              ar_ri->y-= 60;
              UpdateDescription();
            }
            return true;
    case 4: /* down */
            if (menu_type== KD_MENU_GAME && pos< 1)
            { pos++;
              ar_ri->y+= 60;
              UpdateDescription();
            }
            return true;
  }
  
  return false;
}


bool KD_MenuController::display()
{ 
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif   
  
  Display::clearScreen();
  back->Display();
  
  if (menu_type== KD_MENU_GAME)
  { text_font->xycenteredprintf (SCR_HW, 220, "Survival");
    text_font->xycenteredprintf (SCR_HW, 280, "Double Duel");
    ar_ri->Display();
  }

  return true;
}


bool KD_MenuController::quit()
{
  CLOSEMUSIC();
  KD_EventManager::getEventManager()->deleteAllEvents();  
  DELETE (Title);
  
KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  
//  delete main_font;
  DELETE (mini_font);  
  DELETE (text_font);
  /*printf ("%p\n", ar_ri);*/
  DELETE (ar_ri);
  DELETE (ar_r);
  //ar_r[0].~KD_Sprite();

  return KD_Controller::quit();
}


void KD_MenuController::SetMenuType (short menu_type_)
{ menu_type= menu_type_;
}


void KD_MenuController::UpdateDescription()
{ if (Description!= NULL) 
    Description->RemoveText(); 
  NEW (Description, KD_MessageText (DESCRIBE_TEXT[pos], mini_font, SCR_HW, 420));
  Description->activateEvent();
}
