#include "../global.h"

#include "../Controller/KDApplication.h"
#include "../../KDpp/Controller/EventManager.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#include "MenuController.h"
#include "../Names.h"
#include "../Video/Event/AnimTextEvent.h"
#include "../Video/Background.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"
#include "../../KDpp/Sound/Music.h"

static char* DESCRIBE_TEXT[]= 
  { "Solo game",
    "2 players vs",
    "Options" };
    
short KD_MenuController::menu_type= KD_MENU_GAME;

KD_MenuController::KD_MenuController(): KD_Controller(), KD_ResourceSet()
{ }

KD_MenuController::~KD_MenuController()
{ }


bool KD_MenuController::Init()
{ 
  BindKeyDown(SDLK_ESCAPE, 1);
  BindKeyDown(SDLK_SPACE, 2); 
  BindKeyDown(SDLK_RETURN, 2);
  BindKeyDown(SDLK_UP, 3);
  BindKeyDown(SDLK_DOWN, 4);

  main_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font");
  text_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("text font");
  mini_font = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("main font");

#ifndef NO_MUSIC
  music = new KD_Music();
#endif

  LoadResourceFile(KD_KDApplication::GetArtFile("menu.acc/menu.txt"));
  
  ar_r=(KD_Sprite *)GetResource("rightarrow");
  
  return true;
}


bool KD_MenuController::ProcessEvent(int value)
{ switch(value)
  { case 1: KD_Application::GetApplication()->SendStopEvent(); return true;
    case 2: if (menu_type== KD_MENU_GAME) 
            { switch (pos)
              { case 0: 
                  KD_Application::GetApplication()->DisableController(this);
                  KD_Application::GetApplication()->EnableController ("Charsel");
                  break;
                case 1: 
                  KD_Application::GetApplication()->DisableController(this);
                  KD_Application::GetApplication()->EnableController ("Charsel2");
                  break;
                case 2:
                  KD_Application::GetApplication()->DisableController(this);
                  KD_Application::GetApplication()->EnableController ("Controls");
              }
              return true;
            } else assert (0);
    case 3: /* up */
            if (menu_type== KD_MENU_GAME && pos> 0)
            { pos--;
              ar_ry -= 60;
              UpdateDescription();
            }
            return true;
    case 4: /* down */
            if (menu_type== KD_MENU_GAME && pos< 2)
            { pos++;
              ar_ry+= 60;
              UpdateDescription();
            }
            return true;
  }
  
  return false;
}


bool KD_MenuController::Display()
{ 
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif   
  
  if (menu_type== KD_MENU_GAME)
  { text_font->xycenteredprintf (SCR_HW, 220, "Survival");
    text_font->xycenteredprintf (SCR_HW, 280, "Double Duel");
    text_font->xycenteredprintf (SCR_HW, 340, "Options");
    ar_ri->Display(ar_rx,ar_ry);
  }

  return true;
}


bool KD_MenuController::Quit()
{
  DeleteAllEvents();

  ReleaseResource("rightarrow");

  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("text font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("main font");

#ifndef NO_MUSIC
  delete music;
#endif

  return true;
}


void KD_MenuController::SetMenuType (short menu_type_)
{ menu_type= menu_type_;
}


void KD_MenuController::UpdateDescription()
{ if (Description!= NULL) 
    Description->RemoveText(); 
  NEW (Description, KD_MessageText (DESCRIBE_TEXT[pos], mini_font, SCR_HW, 420));
  Description->ActivateEvent();
}

bool KD_MenuController::OnEnable()
{
#ifndef NO_MUSIC
	music->Load(KD_KDApplication::GetArtFile(MUSIC_NAME[KD_MUS_INTRO]).c_str());
	music->PlayMusic();
#endif

  ar_ri = (KD_SpriteInstance *)ar_r->createInstance();
  ar_rx = 120;
  ar_ry = 190;
  ar_ri->setAnim(0);
    
  Description= NULL;
  pos= 0;
  UpdateDescription();

  if (menu_type== KD_MENU_GAME)
  NEW (Title, KD_BouncingText ("Game select", main_font, SCR_HW, 90));
  
	Title->ActivateEvent();
	AddEvent(Title);

	return true;
}

bool KD_MenuController::OnDisable()
{
#ifndef NO_MUSIC
	music->StopMusic();
	music->CloseMusic();
#endif

	ar_r->deleteInstance(ar_ri);
  DELETE (Title);

	return true;
}

bool KD_MenuController::Process()
{
	return false;
}
