#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//#include "../util/direct.h"
#include "Background.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/DisplayableInstance.h"
#include "../../KDpp/Video/DisplayableResource.h"
//#include "../../KDpp/Video/magemanager.h"

#define X_SIZE 1280
#define Y_SIZE 960
#define X_DELTA 100
#define Y_DELTA 50
#define X_WAIT 1200
#define Y_WAIT 100
#define X_SPEED 3
#define Y_SPEED 0.8
#define BACK_FPS 50

//KD_Background* KD_Background::p_background= NULL;

KD_Background::KD_Background(KD_DisplayableResource *spr)
{ InitBackground(spr);
}


KD_Background::~KD_Background()
{ if (backgr!= NULL)
  { //KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
    //assert (image_manager);
    //if (image_manager)
    //  image_manager->releaseImage (back);
	spr->deleteInstance(backgr);
    backgr= NULL;
  }
}


void KD_Background::InitBackground(KD_DisplayableResource *spr)
{ for (signed index= 0; index< KD_BACKGROUND_SPR/ 4; index++)
  { X_S[index   ]= (index* 4*640  / KD_BACKGROUND_SPR)+ 640  ;
    X_S[index+ 5]= (index* 4*640  / KD_BACKGROUND_SPR)+ 640*2;
    X_S[index+10]= (index* 4*640  / KD_BACKGROUND_SPR)+ 640*2;
    X_S[index+15]= (index* 4*640  / KD_BACKGROUND_SPR)+ 640  ;     
    Y_S[index   ]= (index* 4*2*480/ KD_BACKGROUND_SPR)       ;
    Y_S[index+ 5]= (index* 4*2*480/ KD_BACKGROUND_SPR)       ;
    Y_S[index+10]= (index* 4*2*480/ KD_BACKGROUND_SPR)+ 480  ;
    Y_S[index+15]= (index* 4*2*480/ KD_BACKGROUND_SPR)+ 480  ;    
  }
  
//  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
//  assert (image_manager);
  
//  TACCRes acc;
//  signed res;
//  res= acc.LoadACC("art/title.acc");
//  assert (res== 0);
//  if (res== 0)
//  { image_manager->Load (&acc, "title1.png");
//    back= image_manager->getImage ("title1.png");
//    assert (back);
//  }
  this->spr = spr;
  backgr = spr->createInstance();
}


void KD_Background::Display()
{ float incr= (Display::GetTimeElapsed())* 100;

  for (signed index= 0; index< KD_BACKGROUND_SPR; index++)
  { X_S[index]-= incr* X_SPEED;
    Y_S[index]-= incr* Y_SPEED;
    if (X_S[index]< -X_SIZE/ 2) X_S[index]+= X_SIZE;
    if (Y_S[index]< -Y_SIZE/ 2) Y_S[index]+= Y_SIZE;
     
    backgr->Display((short) X_S[index], (short) Y_S[index]);
  }
}


/*KD_Background* KD_Background::GetBackground()
{ if (p_background== NULL) p_background= new KD_Background();
  return p_background;
}*/
