#include <assert.h>
#include "Controller/BackgroundController.h"
#include "Controller/CharSelectController.h"
#include "Controller/CharSelect2Controller.h"
#include "Controller/ControlsController.h"
#include "Controller/DuelController.h"
#include "Controller/HighScoresController.h"
#include "Controller/KDApplication.h"
#include "Controller/MenuController.h"
#include "Controller/SurvivalController.h"
#include "Controller/TitleController.h"
#include "Game/ControlsConfig.h"
#include "../KDpp/Resources/ArchiveManager.h"
#include "../KDpp/Resources/ResourceManager.h"
#include "../KDpp/Resources/GlobalResourceSet.h"
#include "../KDpp/Tools/FilePath.h"
#include "../KDpp/Video/Display.h"
#include "Tools/ACCArchiveReader.h"


int Init(KD_KDApplication* app)
{
	KD_ArchiveManager::RegisterArchiveFormat("acc", CreateACCArchiveReader);
 
	app->InitFromConfigFile();
	Display::SetApplicationName("Krystal Drop!");
	Display::SetApplicationIcon("art/kdrop.ico");

	KD_ControlsConfig::GetSingleton()->InitFromConfigFile();

	app->RegisterController("TitleController", new KD_TitleController());
	app->RegisterController("HighScores", new KD_HighScoresController());
	app->RegisterController("Menu", new KD_MenuController());
	app->RegisterController("Background", new KD_BackgroundController());
	app->RegisterController("Charsel", new KD_CharSelectController());
	app->RegisterController("Charsel2", new KD_CharSelect2Controller());
	app->RegisterController("Survival", new KD_SurvivalController());
	app->RegisterController("Duel", new KD_DuelController());
	app->RegisterController("Controls", new KD_ControlsController());

	app->EnableController("Background", KD_LAST_POS);
	app->EnableController("TitleController", KD_FRONT_POS);

	return 0;
}


void Close(KD_KDApplication* app)
{
	app->UnregisterController("Controls");
	app->UnregisterController("Menu");
	app->UnregisterController("HighScores");
	app->UnregisterController("Background");
	app->UnregisterController("Charsel");
	app->UnregisterController("Charsel2");
	app->UnregisterController("Survival");
	app->UnregisterController("Duel");
	app->UnregisterController("TitleController");

	KD_ControlsConfig::GetSingleton()->Close();

	app->Quit();
}


int main (int argc, char* argv[])
{
	KD_KDApplication* app= KD_KDApplication::GetApplication();

	if (Init(app)==-1) return -1;
	if (!app->Loop()) return -1;
	Close(app);

	delete app;
	return 0;
}
