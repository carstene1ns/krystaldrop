#include <assert.h>

#include "Application.h"
#include "DuelController.h"
#include "../video/font.h"
#include "../video/gem.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../util/direct.h"
#include "../game/parameter.h"
#include "../game/hand.h"
#include "../game/row.h"
#include "../video/Display.h"
#include "../game/set.h"

#define WIDTH 12
#define HEIGHT 14
#define MAX_IN_HAND 8

KD_DuelController::KD_DuelController(): KD_Controller()
{

}

KD_DuelController::~KD_DuelController()
{

}

#define KD_A_QUIT    1
#define KD_A_ADDLINE 2
#define KD_A_TAKEGEM 3
#define KD_A_DROPGEM 4

#define KD_A_LEFT    6
#define KD_A_RIGHT   7




bool KD_DuelController::init()
{
	
/* debug */
param= new KD_Parameters();
param->SetVideoParameters (28, 32, 200, 0);
param->SetGameParameters (3, 0, -1, 0, 1, 1, -1, -1);
set= new KD_Set(WIDTH, HEIGHT, MAX_IN_HAND, param);
/* */


	// Never use action 0 because it's the void action
	bindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
	bindKeyDown(SDLK_SPACE,  KD_A_ADDLINE);
	bindKeyDown(SDLK_UP,     KD_A_DROPGEM);
	bindKeyDown(SDLK_DOWN,   KD_A_TAKEGEM);
    
	bindKeyDown(SDLK_LEFT,   KD_A_LEFT);
	bindKeyDown(SDLK_RIGHT,  KD_A_RIGHT);
  

	TACCRes *accFile = new TACCRes();
    
    signed res;
	res= accFile->LoadACC("gems.acc");
//	res= accFile->LoadACC("immp.acc");
    assert (!res);
    
	blue_spr= new KD_Sprite();
    assert (blue_spr);
	res= blue_spr->Load(accFile,"b.txt");
    assert (res);
    /* return value ? */
	delete accFile;

	smallFont = Display::Slapstick->resize(0.5);
	
	return true;
}

bool KD_DuelController::processEvent(int value)
{ static KD_Gem* g;
  static KD_Gem* gtab[WIDTH+1];
  
	switch(value)
	{
		case KD_A_ADDLINE:
		{
          int index;
          
          for (index= 0; index< WIDTH; index++)
          {
      	        g= new KD_Gem(set, blue_spr, 1);
                g->setFramesPerSeconds (10);
                gtab[index]=g;
          }
          
		     printf ("AddLineAtTop %d\n", set->AddLineAtTop (gtab));
			return true;
		}
			
	   case KD_A_TAKEGEM:
	        printf ("TakeGems %d\n", set->TakeGems());
	        return true;
	        
	   case KD_A_DROPGEM:
	        printf ("DropGems %d\n", set->DropGems());
	        return true;
              
       case KD_A_LEFT:
            set->MoveLeft(); return true;
       case KD_A_RIGHT:
           set->MoveRight(); return true;
	}

	return false;
}

bool KD_DuelController::display()
{ assert (set);

  Display::clearScreen();
  Display::Slapstick->xyprintf(50,50,"Krystal Drop \n on the way !\n \n Whaow!\n1234567890");
  smallFont->xyprintf(50,450,"The same font,\n but smaller");  
  
  set->Update();
  
  if (param->IsNeedClashTest())
    /* and we can..*/    
    if (!param->IsRemoving())
    { set->TestBurstStart();
      param->ClearNeedClashTest();
    }
  
  KD_Gem* gem= set->GetFirstGem();
  while (gem!= NULL)
  {
    gem->Display();
    gem= set->GetNextGem();
  }

  set->RemoveGems();
  
  return true;
}

bool KD_DuelController::quit()
{
  return true;
}
