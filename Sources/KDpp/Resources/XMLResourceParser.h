#ifndef NO_XML
#ifndef XMLResourceParser_H
#define XMLResourceParser_H

#include "../Tools/defines.h"

#include "../Tools/XMLParser.h"

class KD_Resource;
class KD_ResourceSet;
class KD_FilePath;

/**
	Class used to read xml-base sprite files.
  */
class DllExport KD_XMLResourceParser : public KD_XMLParser
{
	KD_ResourceSet *dest;

	string lastStartElement;

	KD_Resource *currentResource;

	string currentResourceName;
	KD_FilePath currentFilePath;

public:
	/**
		Default constructor.
	*/
	KD_XMLResourceParser();

	/**
		Opens the XMLFile fileName.
	*/
	KD_XMLResourceParser(const KD_FilePath &fileName, KD_ResourceSet *dest);

	/**
		Destructor, it frees the space allocated for the file.
	*/
	virtual ~KD_XMLResourceParser();

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
