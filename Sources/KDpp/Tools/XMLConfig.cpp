#include "XMLConfig.h"

#include "Logfile.h"

#include <assert.h>

#ifdef WIN32
#define snprintf _snprintf
#endif


KD_XMLConfig::KD_XMLConfig()
{
	configDocument = 0;
	rootNode = 0;
}

KD_XMLConfig::~KD_XMLConfig()
{
	Unload();
}

bool KD_XMLConfig::Load()
{
	/* now, look for user-specific configuration file */  
	char user_file[128];
#ifndef WIN32
	/* find the user's home, the UNIX way */
	char* home= getenv("HOME");
#else
	/* find the user's home, the Windoze way */
	/* Document Settings\username\ ? */
	//string hometemp = getenv("USERPROFILE");
	char* home= ".";
#endif

	if (home != NULL)
	{
		snprintf (user_file, 128, "%s/kdrop.xml", home);
		user_file[127]= 0;
		if ( Load(user_file)== true)
			return true;
		else
		{
			// try .kdroprc instead
			snprintf (user_file, 128, "%s/.kdroprc", home);
			user_file[127]= 0;
			if (Load(user_file)== true)
				return true;
			else
			{
				// last chance
				if (Load("kdrop.ini")==true)
					return true;
				else
				{
				
#ifndef WIN32
					/* for UNIXes system, try /etc/kdrop.xml first */
					if (Load("/etc/kdrop.xml")==true);
						return true;
#endif
				}
			}
		}
	}
	else Load("kdrop.xml");
	return false;
}

bool KD_XMLConfig::Load(const KD_FilePath &xmlFileName)
{
	configDocument = xmlParseFile(xmlFileName.GetFullPath().c_str());

	if (configDocument == NULL)
		return false;

	rootNode = xmlDocGetRootElement(configDocument);

	if (rootNode == NULL)
	{
		KD_LogFile::printf2("Warning! Could not parse config file %s: no root node in XML document.\n",xmlFileName.GetFullPath().c_str());
		xmlFreeDoc(configDocument);
		return false;
	}

	if (xmlStrcmp(rootNode->name, (const xmlChar *) "config"))
	{
		KD_LogFile::printf2("Warning! Root node of file %s is not of type \"config\".\nTrying to continue parsing.\n",xmlFileName.GetFullPath().c_str());
	}

	return true;
}

bool KD_XMLConfig::Unload()
{
	if (configDocument)
	{
		xmlFreeDoc(configDocument);
		configDocument=0;
	}
	return true;
}

xmlDocPtr KD_XMLConfig::getConfigDocument()
{
	return configDocument;
}

xmlNodePtr KD_XMLConfig::getRootNode()
{
	return rootNode;
}

xmlNodePtr KD_XMLConfig::FindFirstByName(xmlNodePtr node, string name)
{
	assert(node);

	xmlNodePtr child;
	child = node->children;
	while (child != NULL)
	{
		if ((!xmlStrcmp(child->name, (const xmlChar *)name.c_str())))
		{
			return child;
		}
		 
		child = child->next;
	}
	return NULL;
}

string KD_XMLConfig::getNodeText(xmlNodePtr node)
{
	xmlChar *key = xmlNodeListGetString(configDocument, node->xmlChildrenNode, 1);
	string txt = (char *)key;
    xmlFree(key);
 	return txt;
}

string KD_XMLConfig::getAttributeFromNode(xmlNodePtr xmlnode, const string &name)
{
	xmlAttrPtr prop = xmlnode->properties;

	for (; prop!=0; prop = prop->next)
	{
		if (xmlStrEqual(prop->name, reinterpret_cast<const xmlChar*>(name.c_str())))
		{
			xmlChar *key = xmlNodeListGetString(configDocument, prop->xmlChildrenNode, 1);
			string txt = (char *)key;
			xmlFree(key);
 			return txt;
		}
	}
	return "";
}

bool KD_XMLConfig::setAttributeFromNode(xmlNodePtr xmlnode, const string &name, const string &value)
{
	xmlSetProp(xmlnode, reinterpret_cast<const xmlChar*>(name.c_str()) , reinterpret_cast<const xmlChar*>(value.c_str()));
		
	return true;
	/*for (; prop!=0; prop = prop->next)
	{
		if (xmlStrEqual(prop->name, reinterpret_cast<const xmlChar*>(name.c_str())))
		{
			xmlChar *key = xmlNodeListGetString(configDocument, prop->xmlChildrenNode, 1);
			string txt = (char *)key;
			xmlFree(key);
 			return true;
		//}
	}
	return false;*/
}

bool KD_XMLConfig::Save(const KD_FilePath &xmlFileName)
{
	if (!xmlFileName.IsArchived())
	{
		xmlSaveFormatFile (xmlFileName.GetFullPath().c_str(), configDocument, 1);
		return true;
	}
	else
		return false;
}

bool KD_XMLConfig::getOpenGL()
{
	assert(rootNode);

	string resStr = getNodeText(FindFirstByName(FindFirstByName(FindFirstByName(rootNode, "global"),"video"),"opengl"));
	
	if (resStr[0]=='Y' || resStr[0]=='y')
		return true;
	else 
		return false;
}

bool KD_XMLConfig::GetFullScreen()
{
	assert(rootNode);

	string resStr = getNodeText(FindFirstByName(FindFirstByName(FindFirstByName(rootNode, "global"),"video"),"fullscreen"));
	
	if (resStr[0]=='Y' || resStr[0]=='y')
		return true;
	else 
		return false;
}

bool KD_XMLConfig::getEnableSound()
{
	assert(rootNode);

	string resStr = getNodeText(FindFirstByName(FindFirstByName(FindFirstByName(rootNode, "global"),"sound"),"enable"));
	
	if (resStr[0]=='Y' || resStr[0]=='y')
		return true;
	else 
		return false;
}

int KD_XMLConfig::getSoundFrequency()
{
	assert(rootNode);

	string resStr = getNodeText(FindFirstByName(FindFirstByName(FindFirstByName(rootNode, "global"),"sound"),"frequency"));
	
	return atoi(resStr.c_str());
}

int KD_XMLConfig::getSoundBits()
{
	assert(rootNode);

	string resStr = getNodeText(FindFirstByName(FindFirstByName(FindFirstByName(rootNode, "global"),"sound"),"bits"));
	
	return atoi(resStr.c_str());
}

bool KD_XMLConfig::getStereoSound()
{
	assert(rootNode);

	string resStr = getNodeText(FindFirstByName(FindFirstByName(FindFirstByName(rootNode, "global"),"sound"),"stereo"));
	
	if (resStr[0]=='Y' || resStr[0]=='y')
		return true;
	else 
		return false;
}
