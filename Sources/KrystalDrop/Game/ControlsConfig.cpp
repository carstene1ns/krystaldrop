#include "ControlsConfig.h"

#include "../../KDpp/Controller/Application.h"
#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Controller/UserInterface/Keyboard.h"
#include "../../KDpp/Tools/XMLConfig.h"

#ifdef WIN32
#define snprintf _snprintf
#endif


KD_ControlsConfig *KD_ControlsConfig::singleton = NULL;

KD_ControlsConfig *KD_ControlsConfig::GetSingleton()
{
	if (singleton== NULL)
		singleton = new KD_ControlsConfig();

	return singleton;
}

KD_ControlsConfig::KD_ControlsConfig()
{
	for (int i=0; i<KD_NbKeysDefined; i++)
		keyCode[i]=0;
}

KD_ControlsConfig::~KD_ControlsConfig()
{

}

void KD_ControlsConfig::getKeyCodeFromConfig(const string &tag, KD_Keys key)
{
	KD_XMLConfig *config = KD_Application::GetApplication()->GetConfigFile();
	
	xmlNodePtr controlsNode = config->FindFirstByName(config->FindFirstByName(config->GetRootNode(), "game"),"controls");

	string type = config->GetAttributeFromNode(config->FindFirstByName(controlsNode, tag),"type");
	string code = config->GetAttributeFromNode(config->FindFirstByName(controlsNode, tag),"code");

	if (type=="keyboard")
	{
		keyCode[key] = atoi(code.c_str());
		controlKind[key] = CONTROLLER_EVENT_KEYDOWN;
	}
}

void KD_ControlsConfig::SetKeyCode(KD_Keys key, int myKeyCode)
{
	keyCode[key] = myKeyCode;

	KD_XMLConfig *config = KD_Application::GetApplication()->GetConfigFile();
	xmlNodePtr controlsNode = config->FindFirstByName(config->FindFirstByName(config->GetRootNode(), "game"),"controls");

	string xmlName="";

	switch (key)
	{
	case p1up:
		xmlName = "p1up";
		break;
	case p1down:
		xmlName = "p1down";
		break;
	case p1left:
		xmlName = "p1left";
		break;
	case p1right:
		xmlName = "p1right";
		break;
	case p1extra:
		xmlName = "p1extra";
		break;
	case p2up:
		xmlName = "p2up";
		break;
	case p2down:
		xmlName = "p2down";
		break;
	case p2left:
		xmlName = "p2left";
		break;
	case p2right:
		xmlName = "p2right";
		break;
	case p2extra:
		xmlName = "p2extra";
		break;

	}
	
	char buf[128];
	snprintf(buf, 127, "%d",myKeyCode);
  buf[127]= 0;

	config->SetAttributeFromNode(config->FindFirstByName(controlsNode,xmlName) , "code", buf);
	config->SetAttributeFromNode(config->FindFirstByName(controlsNode,xmlName) , "type", "keyboard");
	
#ifndef WIN32
	char* home= getenv ("HOME");
#else
	// # we should look for user home directory
  static const char home[]=".";
#endif

	snprintf (buf, 127, "%s/kdrop.xml", home);
	buf[127]= 0;

	config->Save(buf);
}


void KD_ControlsConfig::InitFromConfigFile()
{
	getKeyCodeFromConfig("p1up",    p1up);
	getKeyCodeFromConfig("p1down",  p1down);
	getKeyCodeFromConfig("p1left",  p1left);
	getKeyCodeFromConfig("p1right", p1right);
	getKeyCodeFromConfig("p1extra", p1extra);
	getKeyCodeFromConfig("p2up",    p2up);
	getKeyCodeFromConfig("p2down",  p2down);
	getKeyCodeFromConfig("p2left",  p2left);
	getKeyCodeFromConfig("p2right", p2right);
	getKeyCodeFromConfig("p2extra", p2extra);
}


void KD_ControlsConfig::Close()
{
	delete singleton;
}

int KD_ControlsConfig::GetControlCode(KD_Keys key)
{
	return keyCode[key];
}

int KD_ControlsConfig::GetControlKind(KD_Keys key)
{
	return controlKind[key];
}

string KD_ControlsConfig::GetControlName(KD_Keys key)
{
	string ret;

	if (controlKind[key] == CONTROLLER_EVENT_KEYDOWN)
	{
		ret =  KD_Keyboard::GetKeyName(keyCode[key]);
	}
	else
	{
		// joystick to do
		ret = "";
	}
	return ret;
}
