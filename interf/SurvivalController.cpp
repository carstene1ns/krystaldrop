#include "SurvivalController.h"

#include "Application.h"
#include "../video/Display.h"
#include "../video/font.h"

#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../util/direct.h"

KD_SurvivalController::KD_SurvivalController() : KD_Controller()
{
}

KD_SurvivalController::~KD_SurvivalController()
{
}

void KD_SurvivalController::loadSprites()
{
	TACCRes *accFile;
	accFile = new TACCRes();
	accFile->LoadACC("border.acc");

	horizontalBar = new KD_Sprite();
	horizontalBar->Load(accFile,"horizontalbar.txt");
	verticalBar = new KD_Sprite();
	verticalBar->Load(accFile,"verticalbar.txt");
	upleftBar = new KD_Sprite();
	upleftBar->Load(accFile,"upleftcorner.txt");
	uprightBar = new KD_Sprite();
	uprightBar->Load(accFile,"uprightcorner.txt");

	delete accFile;

	accFile = new TACCRes();
	accFile->LoadACC("clown.acc");
	clown = new KD_Sprite();
	clown->Load(accFile,"clown.txt");
	delete accFile;
	/*TACCEditMem *tacc = new TACCEditMem();
	tacc->AddEntry("horizontalbar.txt",1000);
	tacc->AddEntry("horizontalbar.png",1000);
	tacc->AddEntry("verticalbar.txt",1000);
	tacc->AddEntry("verticalbar.png",1000);
	tacc->AddEntry("upleftcorner.txt",1000);
	tacc->AddEntry("upleftcorner.png",1000);
	tacc->AddEntry("uprightcorner.txt",1000);
	tacc->AddEntry("uprightcorner.png",1000);
	tacc->SaveACC("border.acc");*/
	/*TACCEditMem *tacc = new TACCEditMem();
	tacc->AddEntry("clown.txt",1000);
	tacc->AddEntry("clown_idle 01.png",1000);
	tacc->AddEntry("clown_idle 02.png",1000);
	tacc->AddEntry("clown_idle 03.png",1000);
	tacc->AddEntry("clown_idle 04.png",1000);
	tacc->AddEntry("clown_idle 05.png",1000);
	tacc->AddEntry("clown_idle 06.png",1000);
	tacc->AddEntry("clown_idle 07.png",1000);
	tacc->AddEntry("clown_idle 08.png",1000);
	tacc->AddEntry("clown_idle 09.png",1000);
	tacc->AddEntry("clown_idle 10.png",1000);
	tacc->SaveACC("clown.acc");*/
}

bool KD_SurvivalController::init()
{
	loadSprites();

	// Never use action 0 because it's the void action
	bindKeyDown(SDLK_ESCAPE, 1);
	bindKeyDown(SDLK_UP, 2);
	bindKeyDown(SDLK_DOWN, 3);

	table.setWidth(9);
	table.setHeight(12);
	table.setGemSize(32);
	table.setPosition(100,50);

	table.setHorizontalBar(horizontalBar);
	table.setVerticalBar(verticalBar);
	table.setUpperLeftBar(upleftBar);
	table.setUpperRightBar(uprightBar);

	table.setClownSprite(clown);

	return true;
}

bool KD_SurvivalController::processEvent(int value)
{
/*	switch(value)
	{
		case 1:
		case 2:
		case 3:
	}*/

	return false;
}

bool KD_SurvivalController::display()
{
	Display::clearScreen();

	table.DisplayBorders();

	return true;
}

bool KD_SurvivalController::quit()
{

	return true;
}
