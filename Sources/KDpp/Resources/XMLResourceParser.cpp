#ifndef NO_XML
#include "XMLResourceParser.h"

#include "Resource.h"
#include "ResourceSet.h"

KD_XMLResourceParser::KD_XMLResourceParser() : KD_XMLParser()
{
	currentResource = NULL;
}


KD_XMLResourceParser::KD_XMLResourceParser(const KD_FilePath &fileName, KD_ResourceSet *dest) : KD_XMLParser(fileName)
{
	this->dest = dest;
	currentResource = NULL;
}

KD_XMLResourceParser::~KD_XMLResourceParser()
{

}

void KD_XMLResourceParser::ListenStartElement(string name, map<string,string> attributes)
{
	lastStartElement = name;

	if (name=="resource")
	{
		currentResourceName = attributes["name"];
		currentFilePath = attributes["file"];
		currentFilePath.PrefixDirectory(xmlFileName.GetFullDirectory());
	}
	else if (name=="parameter")
	{
		currentFilePath.AddParameter(attributes["name"], attributes["value"]);
	}
}

void KD_XMLResourceParser::ListenStringElement(string name)
{
}

void KD_XMLResourceParser::ListenEndElement(string name)
{
	if (name=="resource")
	{
		dest->RegisterResource(currentResourceName, currentFilePath);
		currentResource = NULL;
	}
}

#endif
