#include "../KDpp/Controller/Application.h"
#include "../KDpp/Resources/ResourceManager.h"
#include "../KDpp/Resources/GlobalResourceSet.h"
#include "../KDpp/Video/Display.h"

#include "Config.h"

#include "Controller/TitleController.h"
#include "Controller/HighScoresController.h"
#include "Controller/MenuController.h"
#include "Controller/BackgroundController.h"
#include "Controller/CharSelectController.h"
#include "Controller/CharSelect2Controller.h"
#include "Controller/SurvivalController.h"
#include "Controller/DuelController.h"
//#include "Controller/StartWindowController.h"

#include <assert.h>

int Init(KD_Application* app)
{
  KD_Config* Config= KD_Config::GetConfig();
  assert (Config);
  Config->ReadConfiguration();
  
	if (!app->Init()) return -1;
	KD_ResourceManager::InitResourceManager();
	KD_GlobalResourceSet::InitGlobalResourceSet();
	if (!app->InitVideoSystem(640,480,32,false,true) ) return -1;
	if (!app->InitIOSystem() ) return -1;
	Display::setApplicationName("Krystal Drop!");
	Display::setApplicationIcon("art/kdrop.ico");
	if (!app->InitSoundSystem(44100,16,true) ) return -1;

	app->RegisterController("TitleController", new KD_TitleController());
	app->RegisterController("HighScores", new KD_HighScoresController());
	app->RegisterController("Menu", new KD_MenuController());
	app->RegisterController("Background", new KD_BackgroundController());
	app->RegisterController("Charsel", new KD_CharSelectController());
	app->RegisterController("Charsel2", new KD_CharSelect2Controller());
	app->RegisterController("Survival", new KD_SurvivalController());
	app->RegisterController("Duel", new KD_DuelController());
	//app->RegisterController("StartWindowController", new StartWindowController());


	//app->enableController("StartWindowController", KD_FRONT_POS);
	app->enableController("Background", KD_LAST_POS);
	app->enableController("TitleController", KD_FRONT_POS);


	return 0;
}

int Close(KD_Application* app)
{
	app->unRegisterController("Menu");
	app->unRegisterController("HighScores");
	app->unRegisterController("Background");
	app->unRegisterController("Charsel");
	app->unRegisterController("Charsel2");
	app->unRegisterController("Survival");
	app->unRegisterController("Duel");
	app->unRegisterController("TitleController");
	
	//app->unRegisterController("StartWindowController");

	if (!app->CloseSoundSystem() ) return -1;
	if (!app->CloseVideoSystem() ) return -1;
	KD_GlobalResourceSet::CloseGlobalResourceSet();
	KD_ResourceManager::CloseResourceManager();
	if (!app->Quit()) return -1;

	return 0;
}

int main (int argc, char* argv[])
{ 
	KD_Application* app= KD_Application::getApplication();

	if (Init(app)==-1) return -1;
	if (!app->Loop()) return -1;
	if (Close(app)==-1) return -1;

	delete app;
 
	return 0;
}
