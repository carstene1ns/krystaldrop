#include "../global.h"

#include "Application.h"
#include "CharSelect2Controller.h"
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

/* character select (2 players) controller */
KD_CharSelect2Controller::KD_CharSelect2Controller(): KD_Controller()
{ unsigned i;

//  for (i= 0; i< KD_CSC2_NB_SPRI; i++) spri[i]= NULL;
  for (i= 0; i< KD_CSC2_NB_FONT; i++) font[i]= NULL;
  for (i= 0; i< KD_CSC2_NB_IMG ; i++) img [i]= NULL;    
  
  GETBACK (back);
  
  srand (SDL_GetTicks());
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
    { angle1-= Display::timeElapsed* 200; 
      if (angle1< 0) angle1= 0;
    }
    draw_char1= GET_SEL_CHAR(sel_char1);
  }
  else
  { angle1+= Display::timeElapsed* 200;
    if (angle1> 90)
    { old_char1= -1;
      angle1= 180- angle1; // = 90- (angle1- 90);
      draw_char1= GET_SEL_CHAR(sel_char1);
    }
    else draw_char1= old_char1;
  }

  if (old_char2== -1)
  { if (angle2> 0) 
    { angle2-= Display::timeElapsed* 200; 
      if (angle2< 0) angle2= 0;
    }
    draw_char2= GET_SEL_CHAR(sel_char2);
  }
  else
  { angle2+= Display::timeElapsed* 200;
    if (angle2> 90)
    { old_char2= -1;
      angle2= 180- angle2; // = 90- (angle1- 90);
      draw_char2= GET_SEL_CHAR(sel_char2);
    }
    else draw_char2= old_char2;
  }

#ifndef NO_OPENGL
    if (Display::isOpenGL== true)
    { img[draw_char1]->DisplayRotateY (KD_CSC2_XL1, KD_CSC2_YL, angle1);
      img[draw_char2]->DisplayRotateX (KD_CSC2_XL2, KD_CSC2_YL, angle2);
    } else
#endif  
    { img[draw_char1]->Display (KD_CSC2_XL1- angle1* 4, KD_CSC2_YL);
      img[draw_char2]->Display (KD_CSC2_XL2+ angle2* 4, KD_CSC2_YL);
    }
  
/* small pictures */
  short i1, i2, j1, j2;  
  i1= (GET_SEL_CHAR(sel_char1))% 5;
  j1= (GET_SEL_CHAR(sel_char1))/ 5; /* (i,j)= (column, row) */
  i2= (GET_SEL_CHAR(sel_char2))% 5;
  j2= (GET_SEL_CHAR(sel_char2))/ 5; /* (i,j)= (column, row) */

  for (i= 0; i< KD_NB_CHAR/ 2; i++)
  { if (i1== i && j1== 0 && selected_char1== 0)
    { angle[i]+= Display::timeElapsed* 250;
      if (angle[i]>  360.0) angle[i]-= 360.0;
    } else if (i2== i && j2== 0 && selected_char2== 0)
    { angle[i]-= Display::timeElapsed* 250;
      if (angle[i]< -360.0) angle[i]+= 360.0;
    } else angle[i]/= 1.002;
    
    img[i+ KD_NB_CHAR]->DisplayRotateY (KD_CSC2_XS+ i* KD_CSC2_XSD, KD_CSC2_YS+ offset_y, angle[i]);
  }

  for (i= KD_NB_CHAR/ 2; i< KD_NB_CHAR; i++)
  { if (i1== i- KD_NB_CHAR/ 2 && j1== 1 && selected_char1== 0)
    { angle[i]-= Display::timeElapsed* 250;
      if (angle[i]>  360.0) angle[i]-= 360.0;
    } else if (i2== i- KD_NB_CHAR/ 2 && j2== 1 && selected_char2== 0)
    { angle[i]-= Display::timeElapsed* 250;
      if (angle[i]< -360.0) angle[i]+= 360.0;
    } else angle[i]/= 1.002;

   img[i+ KD_NB_CHAR]->DisplayRotateX (KD_CSC2_XS+ (i- KD_NB_CHAR/ 2)* KD_CSC2_XSD, 
                                       KD_CSC2_YS+ KD_CSC2_YSD+ offset_y, angle[i]);
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


bool KD_CharSelect2Controller::init()
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

  short i;
  for (i= 0; i< 2* KD_NB_CHAR; i++)
  { b= image_manager->Load (acc, CHAR_IMG_NAME[i]);
    assert (b);
    if (b== false) return false;
    img[i]= image_manager->getImage(CHAR_IMG_NAME[i]);
  }

  b= image_manager->Load (acc, "borders1p.png");
  assert (b);
  if (b== false) return false;
  img[KD_NB_CHAR* 2]= image_manager->getImage("borders1p.png");

  b= image_manager->Load (acc, "borders2p.png");
  assert (b);
  if (b== false) return false;
  img[KD_NB_CHAR* 2+ 1]= image_manager->getImage("borders2p.png");
  
  b= image_manager->Load (acc, "vs.png");
  assert (b);
  if (b== false) return false;
  img[KD_NB_CHAR* 2+ 2]= image_manager->getImage("vs.png");
  
  DELETE (acc);

  font[0]= Display::Slapstick;
  font[1]= Display::Slapstick->resize(0.5);

  PLAYMUSIC (MUSIC_NAME[KD_MUS_CHARSELECT]);

  bindKeyDown (SDLK_ESCAPE, 1); /*  quit      */
  bindKeyDown (SDLK_v,      2); /*  select P1 */
  bindKeyDown (SDLK_LSHIFT, 2);
  bindKeyDown (SDLK_LCTRL,  2);  
  bindKeyDown (SDLK_c,      3); /* 'right' P1 */
  bindKeyDown (SDLK_x,      4); /* 'left'  P1 */
  bindKeyDown (SDLK_LEFT,   6); /* 'right' P2 */
  bindKeyDown (SDLK_RIGHT,  5); /* 'left'  P2 */
  bindKeyDown (SDLK_RETURN, 7); /*  select P2 */
  bindKeyDown (SDLK_RSHIFT, 7);
  bindKeyDown (SDLK_RCTRL,  7);
  
  Title= new KD_BouncingText ("Character select", font[0], SCR_W/ 2, 90);
  CHECK_ALLOC (Title);
  Title->activateEvent();
  
  Name1= new KD_MessageText (CHAR_NAME[GET_SEL_CHAR(sel_char1)], font[1], KD_CSC2_XN1, KD_CSC2_YN);
  CHECK_ALLOC (Name1);
  Name1->activateEvent();
  
  Name2= new KD_MessageText (CHAR_NAME[GET_SEL_CHAR(sel_char2)], font[1], KD_CSC2_XN2, KD_CSC2_YN);
  CHECK_ALLOC (Name2);
  Name2->activateEvent();
  
  for (i= 0; i< KD_NB_CHAR; i++) angle[i]= 0.0;
  old_char1= old_char2= -1;
  angle1= 90.0; angle2= 90.0;
  selected_char1= selected_char2= 0;
  
  Display::flip();
  Display::flip(); /* init() takes time, the double flip is to be sure 
                      the flash is visible, by resetting timeElapsed */
  Display::Flash();
  
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
  (*name)->activateEvent();
}

bool KD_CharSelect2Controller::processEvent(int value)
{ switch (value)
  { case 1: KD_Application::getApplication()->sendStopEvent();
            return true;
    case 2: if (selected_char1== 1) return true;
            selected_char1= 1; 
            pl_chars[0]= GET_SEL_CHAR (sel_char1);    
            Display::Flash();
            angle[pl_chars[0]]= 1800;
            if (selected_char2== 1)
            {
              KD_Application::getApplication()->DelayedGotoController ("duel", 2000);
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
            Display::Flash();
            angle[pl_chars[1]]= -1800.0;
            if (selected_char1== 1)
            {
              KD_Application::getApplication()->DelayedGotoController ("duel", 2000);
              Title->RemoveText();
            }
            return true;
  }

  return false;
}

bool KD_CharSelect2Controller::display()
{
  Display::clearScreen();
  
  offset_y= (selected_char1== 1 && selected_char2== 1)?offset_y+ 100* Display::timeElapsed:0.0;
  
  assert (back);
  back->Display();
  
#ifdef DISPLAY_FPS
	Display::DisplayFramesPerSecond (12,42+2+2,20);
#endif  
  
  DisplayChars();
  DisplayCursors();
  
  return true;
}


bool KD_CharSelect2Controller::quit()
{ 
  // DELETE (Title); // the title deletes itself
  DELETE (font[1]); 
  CLOSEMUSIC();
  
  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  for (short i= 0; i< 2* KD_NB_CHAR; i++)
    image_manager->releaseImage (CHAR_IMG_NAME[i]);
  image_manager->releaseImage ("borders1p.png");
  image_manager->releaseImage ("borders2p.png");
  image_manager->releaseImage ("vs.png");  

  return KD_Controller::quit();  
}
