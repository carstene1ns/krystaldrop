#include "StartController.h"

#include "Application.h"

#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../util/direct.h"

KD_StartController::KD_StartController() : KD_Controller()
{

}

KD_StartController::~KD_StartController()
{

}


bool KD_StartController::init()
{
	// Warning! Never use action 0!!!!!
	bindKeyDown(SDLK_ESCAPE, 1);
	bindKeyDown(SDLK_UP, 2);
	bindKeyDown(SDLK_DOWN, 3);

	TACCRes *accFile = new TACCRes();
	accFile->LoadACC("clown.acc");

	

	spr = new Sprite();
	Anim *anim = spr->newAnim();
	//anim->addFileImage("slayers03.jpg");
	//anim->addFileImage("slayers02.jpg");
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

	sprInst = new SpriteInstance(spr);
	sprInst->setFramesPerSeconds(10);

	delete accFile;

	return true;
}

bool KD_StartController::processEvent(int value)
{
	switch(value)
	{
		case 1:
			KD_Application::getApplication()->sendStopEvent();
			return true;
		case 2:
			sprInst->y--;
			return true;
		case 3:
			sprInst->y++;
			return true;
	}

	return false;
}

bool KD_StartController::display()
{

	sprInst->Display();

	return true;
}

bool KD_StartController::quit()
{
	delete sprInst;
	delete spr;

	return true;
}
