#ifndef XMLCharacterParser_H
#define XMLCharacterParser_H

#include "../../KDpp/Tools/defines.h"
#include "../../KDpp/Tools/XMLParser.h"
#include "Character.h"

class KD_FilePath;

/**
	Class used to read xml-base sprite files.
  */
class DllExport KD_XMLCharacterParser : public KD_XMLParser
{
	KD_Character *dest;

	string lastStartElement;

	int currentCategory;
	//KD_Resource *currentResource;
	//string currentResourceName;
	//KD_FilePath currentFilePath;

	KD_CharacAnim currentAction;

public:
	/**
		Default constructor.
	*/
	KD_XMLCharacterParser();

	/**
		Opens the XMLFile fileName.
	*/
	KD_XMLCharacterParser(const KD_FilePath &fileName, KD_Character *dest);

	/**
		Destructor, it frees the space allocated for the file.
	*/
	virtual ~KD_XMLCharacterParser();

	/**
		This method should be overwritten by subclasses to get the start of element events.
	*/
	virtual void ListenStartElement(string name, map<string,string> attributes);

	/**
		This method should be overwritten by subclasses to get the strings of element events.
		This method is also called for the string of an XML attribute.
	*/
	virtual void ListenStringElement(string name);

	/**
		This method should be overwritten by subclasses to get the end of element events.
		This method is also called for the end of an XML attribute.
	*/
	virtual void ListenEndElement(string name);

};

#endif
