#ifndef XMLConfig_H
#define XMLConfig_H

#include "defines.h"

extern "C" {
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
//#include <libxml/parser.h>
}

#include "FilePath.h"

/**
	Reads the XML config file for the application.
  */
class DllExport KD_XMLConfig
{
	/// XML Document
	xmlDocPtr configDocument;

	/// Root node
	xmlNodePtr rootNode;


public:
	/**
		Default constructor.
	*/
	KD_XMLConfig();

	/**
		Destructor, it frees the space allocated for the file.
	*/
	virtual ~KD_XMLConfig();

	/**
		Opens the XMLFile named "kdrop.xml" and located in the current user directory or several other cool places in Unix
	*/
	bool Load();

	/**
		Opens the XMLFile named fileName.
	*/
	bool Load(const KD_FilePath &xmlFileName);

	/**
		Closes the XMLFile.
	*/
	bool Unload();

	/**
		Returns the XML document
	*/
	xmlDocPtr GetConfigDocument();

	/**
		Returns the root node
	*/
	xmlNodePtr GetRootNode();

	/**
		Returns the first element in the node "node" with the name "name"
	*/
	static xmlNodePtr FindFirstByName(xmlNodePtr node, string name);

	/**
		Returns the text from the specified node
	*/
	string GetNodeText(xmlNodePtr node);

	/**
		Returns the given attribute value of the given node.
	*/
	string GetAttributeFromNode(xmlNodePtr xmlnode, const string &name);

	/**
		Sets the given attribute value of the given node.
	*/
	bool SetAttributeFromNode(xmlNodePtr xmlnode, const string &name, const string &value);

	/**
		Saves the file
	*/
	bool Save(const KD_FilePath &xmlFileName);

	//{
	/**
		Reads from / Writes to the config file the parameter "global/opengl"
		Warning! Reading from the Config file is slow (XML)
		The value should be read only once and stored
	*/
	bool GetOpenGL();
	bool GetFullScreen();
	bool GetEnableSound();
	int  GetSoundFrequency();
	int  GetSoundBits();
	bool GetStereoSound();
  string GetArtDirectory();
	//}
};

#endif
