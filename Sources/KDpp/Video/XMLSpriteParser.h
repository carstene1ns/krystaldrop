#ifndef NO_XML
#ifndef XMLSpriteParser_H
#define XMLSpriteParser_H

#include "../Tools/defines.h"
#include "../Tools/XMLParser.h"

class KD_Sprite;
class KD_Anim;

/**
	Class used to read xml-base sprite files.
  */
class DllExport KD_XMLSpriteParser : public KD_XMLParser
{
	KD_Sprite *dest;

	string lastStartElement;

	KD_Anim *currentAnim;


public:
	/**
		Default constructor.
	*/
	KD_XMLSpriteParser();

	/**
		Opens the XMLFile fileName.
	*/
	KD_XMLSpriteParser(const KD_FilePath &fileName, KD_Sprite *dest);

	/**
		Destructor, it frees the space allocated for the file.
	*/
	virtual ~KD_XMLSpriteParser();

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
#endif
