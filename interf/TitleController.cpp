#include "assert.h"

#include "Application.h"
#include "TitleController.h"
#include "../util/direct.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/sprite.h"


KD_Title_Krystal::KD_Title_Krystal (short X, short Y, KD_Sprite* spr): KD_SpriteInstance(spr)
{ x= X;
  y= Y;
  state= 0;
  xs_c= 8;
};

KD_Title_Drop::KD_Title_Drop (short X, short Y, KD_Sprite* spr): KD_SpriteInstance(spr)
{ x= X;
  y= Y;
};


void KD_Title_Krystal::onFinishAnim (int animNo)
{ x+= xs_c;

  switch (state)
{ case 0: if (x> 80) { xs_c= -6; state= 1; }
          break;
  case 1: if (x< 40) { xs_c= 3; state= 2; }

  case 2:
  case 3:
  case 4: xs_c++;
          if (x> 70 && xs_c>0) { xs_c= -(4-state-1)*2- 1; state++; }
          break;
  default: xs_c= 0; return;
}
}

void KD_Title_Drop::onFinishAnim (int animNo)
{ if (y<80) y+= 5; else
  if (y<180) y+= (240-y)/32; else y= 180;
}


/* title controller */
KD_TitleController::KD_TitleController(): KD_Controller()
{ title[0]= title[1]= title[2]= NULL;
  spr= new KD_Sprite[3];
  assert (spr);
  first_tick= SDL_GetTicks();
}
  

KD_TitleController::~KD_TitleController()
{ 
}


#define X_SIZE 1280
#define Y_SIZE 960
#define X_DELTA 100
#define Y_DELTA 50
#define X_WAIT 1200
#define Y_WAIT 100
#define X_SPEED 5
#define Y_SPEED 1

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


void KD_TitleController::DisplayBackground()
{ for (signed index= 0; index< KD_TC_BACKGROUND_SPR; index++)
  { X_S[index]-= X_SPEED;
    Y_S[index]-= Y_SPEED;
    if (X_S[index]< -X_SIZE/2) X_S[index]+= X_SIZE;
    if (Y_S[index]< -Y_SIZE/2) Y_S[index]+= Y_SIZE;
      
    title[0]->x= X_S[index];
    title[0]->y= Y_S[index];
    title[0]->Display();
  }
}


bool KD_TitleController::init()
{ signed res;
  bool b;
  
  /* load the graphics */
  TACCRes* accFile= new TACCRes();
  assert (accFile);
  
  res= accFile->LoadACC("art/title.acc");
  assert (res== 0);

  b= spr[0].Load(accFile,"t_anim1.txt"); assert (b);
  b= spr[1].Load(accFile,"t_anim2.txt"); assert (b);  
  b= spr[2].Load(accFile,"t_anim3.txt"); assert (b);
  
  title[0]= new KD_SpriteInstance(&spr[0]); assert (title[0]);
  InitBackgroundXY();
  
  title[1]= new KD_Title_Krystal(-800, 140, &spr[1]); assert (title[1]);  
  title[2]= new KD_Title_Drop(300, -330, &spr[2]);    assert (title[2]);  

  delete accFile;

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 3);

  main_font= Display::Slapstick->resize(0.5);

  return true;
}


bool KD_TitleController::processEvent(int value)
{ switch(value)
  { case 1:  KD_Application::getApplication()->sendStopEvent(); return true;
    default: KD_Application::getApplication()->gotoController ("survival"); return true;
  }
  
  return false;
}


bool KD_TitleController::display()
{ 
  Display::clearScreen();
  
  DisplayBackground();
  assert (title[1]); title[1]->Display();
  assert (title[2]); title[2]->Display();  
  
  unsigned long tick= SDL_GetTicks()- first_tick;
  if (tick> 10000)
    main_font->xyprintf(180,330,"   Press a key\n    to launch\nthe survival mode\n       demo");
  if (tick> 2000)
  if (tick% 1500<950)
  { main_font->xyprintf(10,470, "insert coin");
	main_font->xyrightprintf(630,470, "insert coin");
  }

  return true;
}


bool KD_TitleController::quit()
{
  delete main_font; /* ## ?? */
  delete title[0]; title[0]= NULL;
  delete title[1]; title[1]= NULL;
  delete title[2]; title[2]= NULL;
  delete[] spr;

  return true;
}
