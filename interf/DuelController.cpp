#include "Application.h"
#include "DuelController.h"
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
param= new KD_Parameters(3, 1);
hand= new KD_Hand(6);
row= new KD_Row(5, hand, param);
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

	sprInst= new KD_SpriteInstance(spr);
	sprInst->setFramesPerSeconds(10);
	
	sprInst2= new KD_SpriteInstance(spr);
	sprInst2->setFramesPerSeconds(20);

	delete accFile;
	
	return true;
}

bool KD_DuelController::processEvent(int value)
{
	switch(value)
	{
		case KD_A_ADDLINE:
			return true;
	}

	return false;
}

bool KD_DuelController::display()
{

	sprInst->Display();

	return true;
}

bool KD_DuelController::quit()
{
	delete sprInst;
	delete spr;

	return true;
}
