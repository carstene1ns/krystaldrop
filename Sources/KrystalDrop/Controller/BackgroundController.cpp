#include "../global.h"

#include "../../KDpp/Controller/Application.h"
#include "../../KDpp/Controller/EventManager.h"
#include "BackgroundController.h"
//#include "../util/direct.h"
#include "../Video/Background.h"
#include "../../KDpp/Video/Display.h"

#define ANIM_SIZE 150
#define FLASHTIME 0.25f
#define FLASHMUL (1.0f/FLASHTIME)

KD_BackgroundController::KD_BackgroundController(): KD_Controller(), KD_ResourceSet()
{ 
	
}

KD_BackgroundController::~KD_BackgroundController()
{ 

}


bool KD_BackgroundController::Init()
{ 

	LoadResourceFile("art/title/titleRes.txt");
	back = new KD_Background((KD_DisplayableResource *)GetResource("title1"));

	return true;
}


bool KD_BackgroundController::ProcessEvent(int value)
{ 
	return false;
}

bool KD_BackgroundController::Process()
{
	return false;
}

bool KD_BackgroundController::Display()
{ 
	if (flashTime> 0)
	{ 
		flashTime-= Display::getTimeElapsed();
		if (flashTime< 0) flashTime= 0;
		float col= flashTime* FLASHMUL;
		Display::setClearColor(KD_Color(col, col, col));
	}
	
	Display::clearScreen();

	back->Display();
  
	return true;
}


bool KD_BackgroundController::Quit()
{
	delete back;
	ReleaseResource("title1");
	return true;
}

bool KD_BackgroundController::OnEnable()
{
	return true;
}

bool KD_BackgroundController::OnDisable()
{
	return true;
}

void KD_BackgroundController::Flash()
{
	flashTime= FLASHTIME; 
}
