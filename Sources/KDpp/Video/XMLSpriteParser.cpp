#ifndef NO_XML
#include "XMLSpriteParser.h"

#include "Sprite.h"

KD_XMLSpriteParser::KD_XMLSpriteParser() : KD_XMLParser()
{
	currentAnim = NULL;
}


KD_XMLSpriteParser::KD_XMLSpriteParser(const KD_FilePath &fileName, KD_Sprite *dest) : KD_XMLParser(fileName)
{
	this->dest = dest;
	currentAnim = NULL;
}

KD_XMLSpriteParser::~KD_XMLSpriteParser()
{

}

void KD_XMLSpriteParser::ListenStartElement(string name, map<string,string> attributes)
{
	lastStartElement = name;

	if (name=="sprite")
	{
		dest->setFramePerSec((float)atof(attributes["fps"].c_str()));
	}
	else if (name=="anim")
	{
		currentAnim = dest->newAnim();
	}
	else if (name=="file")
	{
		string imageFile = attributes["name"];
		int x = (int)atof(attributes["xcenter"].c_str());
		int y = (int)atof(attributes["ycenter"].c_str());

		string imagePath;
		/*if (xmlFileName.GetFullDirectory() != "")
			imagePath = xmlFileName.GetFullDirectory()+"/"+imageFile;
		else
			imagePath = imageFile;*/
		imagePath = xmlFileName.GetFullDirectory()+imageFile;

		currentAnim->addFileImage(imagePath,x,y);
	}
	else if (name=="goto")
	{
		currentAnim->setNextAnim((int)atoi(attributes["anim"].c_str()));
	}
	else if (name=="onlyonce")
	{
		currentAnim->setNextAnim(KD_NONEXTANIM);
	}
}

void KD_XMLSpriteParser::ListenStringElement(string name)
{
}

void KD_XMLSpriteParser::ListenEndElement(string name)
{
	if (name=="anim")
	{
		currentAnim = NULL;
	}
}

#endif
