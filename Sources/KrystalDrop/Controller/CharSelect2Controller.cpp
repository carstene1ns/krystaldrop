#include "../global.h"
#include "../Game/ControlsConfig.h"
#include "BackgroundController.h"
#include "CharSelect2Controller.h"
#include "../Controller/KDApplication.h"
#include "../Video/Background.h"
#include "../Video/Event/AnimTextEvent.h"
#include "../../KDpp/Controller/EventManager.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#include "../../KDpp/Sound/Music.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Image.h"
#include "../../KDpp/Video/Sprite.h"

#define M_PI 3.14159265358979323846

#define ANIM_SIZE 150

#define GET_SEL_CHAR(sel_char) ((10+ (10- (sel_char))% 10)% 10)

/* character select (2 players) controller */
KD_CharSelect2Controller::KD_CharSelect2Controller(): KD_Controller(), KD_ResourceSet()
{ unsigned i;

  for (i= 0; i< KD_CSC2_NB_FONT; i++) font[i]= NULL;
  for (i= 0; i< KD_CSC2_NB_IMG ; i++) img [i]= NULL;    
  
  srand (Display::GetTicks());
  sel_char1= rand()% KD_NB_CHAR;
  sel_char2= rand()% KD_NB_CHAR;
  if (sel_char2== sel_char1) sel_char2++;
  
  Title= NULL;
  Name1= Name2= NULL;
}
  

KD_CharSelect2Controller::~KD_CharSelect2Controller()
{ 
}


/* where to place the small pictures */
#define KD_CSC2_XSD 90
#define KD_CSC2_YSD 90
#define KD_CSC2_XS  (SCR_W- (KD_CSC2_XSD* KD_NB_CHAR/ 2))/ 2
#define KD_CSC2_YS  320

/* where to place the large pictures */
#define KD_CSC2_XL1 30
#define KD_CSC2_XL2 354
#define KD_CSC2_YL  50

/* where to place the characters' names */
#define KD_CSC2_XN1 138
#define KD_CSC2_XN2 502
#define KD_CSC2_YN  120

/* where to place vs */
#define KD_CSC2_XVS (SCR_W/ 2- 60)
#define KD_CSC2_YVS (SCR_H/ 2- 30)


/* offset for the borders */
#define KD_CSC2_OFFBOR 3


void KD_CharSelect2Controller::DisplayChars()
{ short i;
  
/* large pictures */
  short draw_char1, draw_char2;  
  if (old_char1== -1)
  { if (angle1> 0) 
    { angle1-= Display::GetTimeElapsed()* 200.0f; 
      if (angle1< 0) angle1= 0;
    }
    draw_char1= GET_SEL_CHAR(sel_char1);
  }
  else
  { angle1+= Display::GetTimeElapsed()* 200.0f;
    if (angle1> 90.0f)
    { old_char1= -1;
      angle1= 180.0f- angle1; // = 90- (angle1- 90);
      draw_char1= GET_SEL_CHAR(sel_char1);
    }
    else draw_char1= old_char1;
  }

  if (old_char2== -1)
  { if (angle2> 0) 
    { angle2-= Display::GetTimeElapsed()* 200.0f; 
      if (angle2< 0) angle2= 0;
    }
    draw_char2= GET_SEL_CHAR(sel_char2);
  }
  else
  { angle2+= Display::GetTimeElapsed()* 200.0f;
    if (angle2> 90.0f)
    { old_char2= -1;
      angle2= 180.0f- angle2; // = 90- (angle1- 90);
      draw_char2= GET_SEL_CHAR(sel_char2);
    }
    else draw_char2= old_char2;
  }

#ifndef NO_OPENGL
    if (Display::GetIsOpenGL()== true)
    { //img[draw_char1]->DisplayRotateY (KD_CSC2_XL1, KD_CSC2_YL, angle1);
		img[draw_char1]->DisplayColorZoom (KD_CSC2_XL1, KD_CSC2_YL + (1-cos(angle1*M_PI/180))*img[draw_char1]->GetHeight()/2, 255,255,255,255, 1, cos(angle1*M_PI/180.0f));
      //img[draw_char2]->DisplayRotateX (KD_CSC2_XL2, KD_CSC2_YL, angle2);
		img[draw_char2]->DisplayColorZoom (KD_CSC2_XL2 + (1-cos(angle2*M_PI/180.0f))*img[draw_char2]->GetWidth()/2, KD_CSC2_YL, 255,255,255,255, cos(angle2*M_PI/180.0f),1);
    } else
#endif  
    { img[draw_char1]->Display (KD_CSC2_XL1- angle1* 4.0f, KD_CSC2_YL);
      img[draw_char2]->Display (KD_CSC2_XL2+ angle2* 4.0f, KD_CSC2_YL);
    }
  
/* small pictures */
  short i1, i2, j1, j2;  
  i1= (GET_SEL_CHAR(sel_char1))% 5;
  j1= (GET_SEL_CHAR(sel_char1))/ 5; /* (i,j)= (column, row) */
  i2= (GET_SEL_CHAR(sel_char2))% 5;
  j2= (GET_SEL_CHAR(sel_char2))/ 5; /* (i,j)= (column, row) */

  for (i= 0; i< KD_NB_CHAR/ 2; i++)
  { if (i1== i && j1== 0 && selected_char1== 0)
    { angle[i]+= Display::GetTimeElapsed()* 250.0f;
      if (angle[i]>  360.0f) angle[i]-= 360.0;
    } else if (i2== i && j2== 0 && selected_char2== 0)
    { angle[i]-= Display::GetTimeElapsed()* 250.0f;
      if (angle[i]< -360.0f) angle[i]+= 360.0f;
    } else angle[i] /= 1+Display::GetTimeElapsed();//angle[i] /= 1.002f;
    
    //img[i+ KD_NB_CHAR]->DisplayRotateY (KD_CSC2_XS+ i* KD_CSC2_XSD, KD_CSC2_YS+ offset_y, angle[i]);
	img[i+ KD_NB_CHAR]->DisplayColorZoom (KD_CSC2_XS+ i* KD_CSC2_XSD, KD_CSC2_YS+ offset_y  + (1-cos(angle[i]*M_PI/180))*img[i+ KD_NB_CHAR]->GetHeight()/2, 255,255,255,255,1, cos(angle[i]*M_PI/180));
  }

  for (i= KD_NB_CHAR/ 2; i< KD_NB_CHAR; i++)
  { if (i1== i- KD_NB_CHAR/ 2 && j1== 1 && selected_char1== 0)
    { angle[i]-= Display::GetTimeElapsed()* 250.0f;
      if (angle[i]>  360.0f) angle[i]-= 360.0;
    } else if (i2== i- KD_NB_CHAR/ 2 && j2== 1 && selected_char2== 0)
    { angle[i]-= Display::GetTimeElapsed()* 250.0f;
      if (angle[i]< -360.0f) angle[i]+= 360.0f;
    } else angle[i] /= 1+Display::GetTimeElapsed();// angle[i]/= 1.002f;

   /*img[i+ KD_NB_CHAR]->DisplayRotateX (KD_CSC2_XS+ (i- KD_NB_CHAR/ 2)* KD_CSC2_XSD, 
                                       KD_CSC2_YS+ KD_CSC2_YSD+ offset_y, angle[i]);*/
	img[i+ KD_NB_CHAR]->DisplayColorZoom(KD_CSC2_XS+ (i- KD_NB_CHAR/ 2)* KD_CSC2_XSD + (1-cos(angle[i]*M_PI/180))*img[i+ KD_NB_CHAR]->GetWidth()/2, KD_CSC2_YS+ KD_CSC2_YSD+ offset_y, 255,255,255,255, cos(angle[i]*M_PI/180),1);
		
  }
  
  img[2* KD_NB_CHAR+ 2]->Display (KD_CSC2_XVS, KD_CSC2_YVS- offset_y* 2);
}


void KD_CharSelect2Controller::DisplayCursors()
{ signed char i, j, k;
  
  i= (GET_SEL_CHAR(sel_char1))% 5;
  j= (GET_SEL_CHAR(sel_char1))/ 5; /* (i,j)= (column, row) */
  k= KD_CSC2_OFFBOR;
  img[2* KD_NB_CHAR]->Display (KD_CSC2_XS+ i* KD_CSC2_XSD- k, 
                               KD_CSC2_YS+ j* KD_CSC2_YSD- k+ offset_y);
  
  i= (GET_SEL_CHAR(sel_char2))% 5;
  j= (GET_SEL_CHAR(sel_char2))/ 5; /* (i,j)= (column, row) */
  img[2* KD_NB_CHAR+ 1]->Display (KD_CSC2_XS+ i* KD_CSC2_XSD- k, 
                                  KD_CSC2_YS+ j* KD_CSC2_YSD- k+ offset_y);
}


bool KD_CharSelect2Controller::Init()
{ 
  LoadResourceFile(KD_KDApplication::GetArtFile("characters/characters.txt"));

  short i;
  for (i= 0; i< 2* KD_NB_CHAR; i++)
    img[i] = (KD_Image *)GetResource(CHAR_IMG_NAME[i]);

  img[KD_NB_CHAR* 2] = (KD_Image *)GetResource("borders1p");
  img[KD_NB_CHAR* 2+ 1] = (KD_Image *)GetResource("borders2p");
  img[KD_NB_CHAR* 2+ 2] = (KD_Image *)GetResource("vs");

  font[0] = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font");
  font[1] = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("main font");

#ifndef NO_MUSIC
  music = new KD_Music();
#endif

  return true;
}


void KD_CharSelect2Controller::ChangeChar (short player, signed short mod)
{ short* sel= (player== 1)?&sel_char1:&sel_char2;
  short* old= (player== 1)?&old_char1:&old_char2;
  KD_MessageText** name= (player== 1)?&Name1:&Name2;
  short x= (player== 1)?KD_CSC2_XN1:KD_CSC2_XN2;
  short y= KD_CSC2_YN;
  
  if (*old== -1) (*old)= GET_SEL_CHAR(*sel);
  (*sel)+= mod;
  if (GET_SEL_CHAR(sel_char1)== GET_SEL_CHAR(sel_char2)) (*sel)+= mod;
  (*name)->RemoveText();
  (*name)= new KD_MessageText(CHAR_NAME[GET_SEL_CHAR(*sel)], font[1], x, y);
  (*name)->ActivateEvent();
  AddEvent((*name));
}

bool KD_CharSelect2Controller::ProcessEvent(int value)
{ switch (value)
  { case 1: KD_Application::GetApplication()->SendStopEvent();
            return true;
    case 2: if (selected_char1== 1) return true;
            selected_char1= 1; 
            pl_chars[0]= GET_SEL_CHAR (sel_char1);
			((KD_BackgroundController*)(KD_Application::GetApplication()->GetController("Background")))->Flash();
            //Display::Flash();
            angle[pl_chars[0]]= 1800.0f;
            if (selected_char2== 1)
            {
              delayedGotoTime = Display::GetTicks();
              enableDelayedGoto = true;
                //KD_Application::GetApplication()->DelayedGotoController ("duel", 2000);
              Title->RemoveText();
            }
            return true;
    case 3: if (selected_char1!= 1) ChangeChar (1, -1); 
            return true;
    case 4: if (selected_char1!= 1) ChangeChar (1, +1); 
            return true;
    case 5: if (selected_char2!= 1) ChangeChar (2, -1); 
            return true;
    case 6: if (selected_char2!= 1) ChangeChar (2, +1); 
            return true;
    case 7: if (selected_char2== 1) return true;
            selected_char2= 1;   
            pl_chars[1]= GET_SEL_CHAR (sel_char2);
			((KD_BackgroundController*)(KD_Application::GetApplication()->GetController("Background")))->Flash();
            //Display::Flash();
            angle[pl_chars[1]]= -1800.0;
            if (selected_char1== 1)
            {
              delayedGotoTime = Display::GetTicks();
              enableDelayedGoto = true;
              //KD_Application::GetApplication()->DelayedGotoController ("duel", 2000);
              Title->RemoveText();
            }
            return true;
  }

  return false;
}

bool KD_CharSelect2Controller::Process()
{
	if (Display::GetTicks() - delayedGotoTime > 2000 && enableDelayedGoto == true)
	{
		KD_Application::GetApplication()->DisableController (this);
		KD_Application::GetApplication()->DisableController ("Background");
		KD_Application::GetApplication()->EnableController ("Duel");
	}

	return false;
}


bool KD_CharSelect2Controller::Display()
{
  //Display::clearScreen();
  
  offset_y= (selected_char1== 1 && selected_char2== 1)?offset_y+ 100* Display::GetTimeElapsed():0.0;
  
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif  
  
  DisplayChars();
  DisplayCursors();
  
  return true;
}


bool KD_CharSelect2Controller::Quit()
{ 
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("main font");

#ifndef NO_MUSIC
  delete music;
#endif
  
  for (short i= 0; i< 2* KD_NB_CHAR; i++)
	  ReleaseResource(CHAR_IMG_NAME[i]);
  ReleaseResource("borders1p");
  ReleaseResource("borders2p");
  ReleaseResource("vs");

  return true;  
}

bool KD_CharSelect2Controller::OnEnable()
{
	delayedGotoTime=0;
	enableDelayedGoto = false;

#ifndef NO_MUSIC
	music->Load(KD_KDApplication::GetArtFile(MUSIC_NAME[KD_MUS_CHARSELECT]).c_str());
	music->PlayMusic();
#endif

	Title= new KD_BouncingText ("Character select", font[0], SCR_W/ 2, 90);
	CHECK_ALLOC (Title);
	Title->ActivateEvent();
	AddEvent(Title);

	Name1= new KD_MessageText (CHAR_NAME[GET_SEL_CHAR(sel_char1)], font[1], KD_CSC2_XN1, KD_CSC2_YN);
	CHECK_ALLOC (Name1);
	Name1->ActivateEvent();
	AddEvent(Name1);

	Name2= new KD_MessageText (CHAR_NAME[GET_SEL_CHAR(sel_char2)], font[1], KD_CSC2_XN2, KD_CSC2_YN);
	CHECK_ALLOC (Name2);
	Name2->ActivateEvent();
	AddEvent(Name2);

	int i;
	for (i= 0; i< KD_NB_CHAR; i++) angle[i]= 0.0;
	old_char1= old_char2= -1;
	angle1= 90.0; angle2= 90.0;
	selected_char1= selected_char2= 0;

    // default binding:
	BindKeyDown (SDLK_ESCAPE, 1); /*  Quit      */
	  
	// custom bindings:  
	KD_ControlsConfig *config = KD_ControlsConfig::GetSingleton();

	BindInput (config->GetControlKind(KD_ControlsConfig::p2up) ,  config->GetControlCode(KD_ControlsConfig::p2up),   2);
	BindInput (config->GetControlKind(KD_ControlsConfig::p2extra),config->GetControlCode(KD_ControlsConfig::p2extra),2);
	BindInput (config->GetControlKind(KD_ControlsConfig::p2left), config->GetControlCode(KD_ControlsConfig::p2left), 4);
	BindInput (config->GetControlKind(KD_ControlsConfig::p2right),config->GetControlCode(KD_ControlsConfig::p2right),3);

	BindInput (config->GetControlKind(KD_ControlsConfig::p1up) ,  config->GetControlCode(KD_ControlsConfig::p1up),   7);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1extra),config->GetControlCode(KD_ControlsConfig::p1extra),7);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1left), config->GetControlCode(KD_ControlsConfig::p1left), 6);
	BindInput (config->GetControlKind(KD_ControlsConfig::p1right),config->GetControlCode(KD_ControlsConfig::p1right),5);

	//Display::flip();
	//Display::flip(); /* Init() takes time, the double flip is to be sure 
	//                    the flash is visible, by resetting timeElapsed */
	//Display::Flash();
	((KD_BackgroundController*)(KD_Application::GetApplication()->GetController("Background")))->Flash();

	return true;
}

bool KD_CharSelect2Controller::OnDisable()
{
#ifndef NO_MUSIC
	music->StopMusic();
	music->CloseMusic();
#endif

	DeleteAllEvents();
	
	return true;
}
