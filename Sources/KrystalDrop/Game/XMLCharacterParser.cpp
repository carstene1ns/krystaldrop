#include "XMLCharacterParser.h"
#include "../../KDpp/Tools/Logfile.h"

KD_XMLCharacterParser::KD_XMLCharacterParser() : KD_XMLParser()
{
	currentCategory = -1;
}


KD_XMLCharacterParser::KD_XMLCharacterParser(const KD_FilePath &fileName, KD_Character *dest) : KD_XMLParser(fileName)
{
	this->dest = dest;
	currentCategory = -1;
}

KD_XMLCharacterParser::~KD_XMLCharacterParser()
{

}

void KD_XMLCharacterParser::ListenStartElement(string name, map<string,string> attributes)
{
	lastStartElement = name;

	if (name=="event")
	{
		string categoryName = attributes["name"];
		if (categoryName=="attack")
		{
			currentCategory = KD_ATTACK;
		}
		else if (categoryName=="strongattack")
		{
			currentCategory = KD_STRONGATTACK;
		}
		else if (categoryName=="attacked")
		{
			currentCategory = KD_ATTACKED;
		}
		else if (categoryName=="danger")
		{
			currentCategory = KD_DANGER;
		}
		else if (categoryName=="winning")
		{
			currentCategory = KD_WINNING;
		}
		else if (categoryName=="loosing")
		{
			currentCategory = KD_LOOSING;
		}
		else
		{
			printf("WARNING! Category unknown in file %s",xmlFileName.GetFullPath().c_str());
			KD_LogFile::printf2("WARNING! Category unknown in file %s",xmlFileName.GetFullPath().c_str());
		}
	}
	else if (name=="action")
	{
		currentAction.animNumber=0;
#ifndef NO_SOUND
		currentAction.voice= NULL;
#endif
		float proba = (float) atof(attributes["probability"].c_str());
		currentAction.proba=proba;
		dest->probaSum[currentCategory] += proba;
	}
	else if (name=="anim")
	{
		currentAction.animNumber=atoi(attributes["number"].c_str());
	}
	else if (name=="voice")
	{
#ifndef NO_SOUND
		currentAction.voice = (KD_Sound*) dest->GetResource(attributes["name"]);
#endif
	}
}

void KD_XMLCharacterParser::ListenStringElement(string name)
{
}

void KD_XMLCharacterParser::ListenEndElement(string name)
{
	if (name=="action")
	{
		dest->characAnims[currentCategory].push_back(currentAction);
	}
	else if (name=="event")
	{
		currentCategory = -1;
	}
}

