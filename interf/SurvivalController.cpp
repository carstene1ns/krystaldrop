#include "SurvivalController.h"

#include "Application.h"
#include "../video/Display.h"
#include "../video/font.h"

#include "../video/sprite.h"
#include "../video/spriteinstance.h"
#include "../util/direct.h"

#define KD_A_QUIT    1
#define KD_A_ADDLINE 2
#define KD_A_TAKEGEM 3
#define KD_A_DROPGEM 4
#define KD_A_REMOVEGEM 5
#define KD_A_LEFT    6
#define KD_A_RIGHT   7

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

	accFile = new TACCRes();
	accFile->LoadACC("gems.acc");
	gem[KD_BLUE] = new KD_Sprite();
	gem[KD_BLUE]->Load(accFile,"b.txt");
	gem[KD_GREEN] = new KD_Sprite();
	gem[KD_GREEN]->Load(accFile,"g.txt");
	gem[KD_RED] = new KD_Sprite();
	gem[KD_RED]->Load(accFile,"r.txt");
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
	bindKeyDown(SDLK_ESCAPE, KD_A_QUIT);
	bindKeyDown(SDLK_LEFT, KD_A_LEFT);
	bindKeyDown(SDLK_RIGHT, KD_A_RIGHT);
	bindKeyDown(SDLK_SPACE, KD_A_ADDLINE);
	bindKeyDown(SDLK_UP,     KD_A_DROPGEM);
	bindKeyDown(SDLK_DOWN,   KD_A_TAKEGEM);

	table.setWidth(9);
	table.setHeight(12);
	table.setGemWidth(32);
	table.setGemHeight(32);
	table.setPosition(100,50);

	table.setHorizontalBar(horizontalBar);
	table.setVerticalBar(verticalBar);
	table.setUpperLeftBar(upleftBar);
	table.setUpperRightBar(uprightBar);

	table.setClownSprite(clown);

	table.setGems(gem);

	table.loadGemsToCome("table.txt");

	table.Init();

	return true;
}

bool KD_SurvivalController::processEvent(int value)
{
	switch(value)
	{
		case KD_A_QUIT:
			KD_Application::getApplication()->sendStopEvent();
			return true;
		case KD_A_LEFT:
			table.MoveLeft();
			return true;
		case KD_A_RIGHT:
			table.MoveRight();
			return true;
		case KD_A_TAKEGEM:
			table.takeGems();
			return true;
		case KD_A_DROPGEM:
			table.dropGems();
			return true;
		case KD_A_ADDLINE:
			table.addLine();
			return true;
	}

	return false;
}

bool KD_SurvivalController::display()
{
	Display::clearScreen();

	table.Display();
	
	return true;
}

bool KD_SurvivalController::quit()
{
	table.deInit();

	return true;
}
