#include "Application.h"
#include "DuelController.h"
#include "../video/gem.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../util/direct.h"
#include "../game/parameter.h"
#include "../game/hand.h"
#include "../game/row.h"

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

    

	


bool KD_DuelController::init()
{
/* debug */
param= new KD_Parameters();
param->SetVideoParameters (30,0);
param->SetGameParameters (1, 0, 1, 1);
hand= new KD_Hand(6);
row= new KD_Row(15, hand, param);
/* */


	// Never use action 0 because it's the void action
	bindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
	bindKeyDown(SDLK_UP,     KD_A_ADDLINE);
	bindKeyDown(SDLK_DOWN,   KD_A_TAKEGEM);

	TACCRes *accFile = new TACCRes();
	accFile->LoadACC("clown.acc");

	spr = new KD_Sprite();
	KD_Anim *anim = spr->newAnim();

	//anim->addFileImage("clown_idle 01.png");
	anim->addFileImageFromACC(accFile,"clown_idle 01.png");
	anim->addFileImageFromACC(accFile,"clown_idle 02.png");
	anim->addFileImageFromACC(accFile,"clown_idle 03.png");
	anim->addFileImageFromACC(accFile,"clown_idle 04.png");
	anim->addFileImageFromACC(accFile,"clown_idle 05.png");
	anim->addFileImageFromACC(accFile,"clown_idle 06.png");
	anim->addFileImageFromACC(accFile,"clown_idle 07.png");
	anim->addFileImageFromACC(accFile,"clown_idle 08.png");
	anim->addFileImageFromACC(accFile,"clown_idle 09.png");
	anim->addFileImageFromACC(accFile,"clown_idle 10.png");

	g1= new KD_Gem(spr, 1);
	g1->setFramesPerSeconds(10);
	
	g2= new KD_Gem(spr, 2);
	g2->setFramesPerSeconds(20);

    g1->y= 0;
    g1->x= 0;
    g2->x= 0;

	delete accFile;
	
	row->AddAtTop (g1);
	
	return true;
}

bool KD_DuelController::processEvent(int value)
{
	switch(value)
	{
		case KD_A_ADDLINE:
		{
      	KD_Gem* g= new KD_Gem(spr, 4);
      	        g->x= 0;
              	g->setFramesPerSeconds(8);
		     printf ("AddAtTop %d\n", row->AddAtTop (g));
			return true;
	    }
			
	   case KD_A_TAKEGEM:
	        printf ("TakeFromBottomn %d\n", row->TakeFromBottom());
	        
	        return true;
	}

	return false;
}

bool KD_DuelController::display()
{ assert (row);

  row->Update();
  KD_Gem* gem= row->GetFirstGem();
  while (gem!= NULL)
  {
    
    gem->Display();
    gem= row->GetNextGem();
  }

	return true;
}

bool KD_DuelController::quit()
{
	delete g1;
	delete g2;
	delete spr;

	return true;
}
