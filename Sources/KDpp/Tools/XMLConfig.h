#ifndef XMLConfig_H
#define XMLConfig_H

#include "defines.h"

extern "C"
{
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
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
    xmlDocPtr GetConfigDocument() const;

    /**
        Returns the root node
    */
    xmlNodePtr GetRootNode() const;

    /**
        Returns the first element in the node "node" with the name "name"
    */
    static xmlNodePtr FindFirstByName(xmlNodePtr node, string name);

    /**
        Returns the text from the specified node
    */
    string GetNodeText(xmlNodePtr node) const;

    /**
        Returns the given attribute value of the given node.
    */
    string GetAttributeFromNode(xmlNodePtr xmlnode, const string &name) const;

    /**
        Sets the given attribute value of the given node.
    */
    bool SetAttributeFromNode(xmlNodePtr xmlnode, const string &name, const string &value);

    /**
        Gets a configuration item as a string. Items are specified as 'a/b/../c'
        Warning, reading from the Config XML file is slow. Values should be read
        only once and stored somewhere.
    */
    string GetConfigurationString (string config_item) const;

    /**
        Saves the file
    */
    bool Save(const KD_FilePath &xmlFileName);
};

#endif
