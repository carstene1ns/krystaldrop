#include "XMLParser.h"

#include <libxml/parserInternals.h>


//#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/util/TransService.hpp>

//#include <string.h>
//#include <iostream.h>
//#include <stdlib.h>

#include "Logfile.h"

/*bool KD_XMLParser::InitXerces()
{
	try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
		char* message = XMLString::transcode(toCatch.getMessage());
		KD_LogFile::printf2("Error during XMLParses Initialization! :\n %s", message);
		delete[] message;
		return false;
    }

	xercesSAXParser = new SAXParser();
	        
	xercesSAXParser->setDoValidation(true);    // optional.
	//xercesSAXParser->setDoNamespaces(true);    // optional

	// this class derives from HandlerBase
	xercesSAXParser->setDocumentHandler(this);
	xercesSAXParser->setErrorHandler(this);

	

	return true;
}

bool KD_XMLParser::closeXerces()
{
	if (xercesSAXParser)
	{
		delete xercesSAXParser;
		xercesSAXParser = NULL;
	}

	XMLPlatformUtils::Terminate();

	return true;
}*/


//void startElement(void *user_data, const CHAR *name, const CHAR **attrs)
void startElement(void *user_data, const xmlChar *name, const xmlChar **attrs)
{
	//printf("%s",name);

	map<string,string> attMap;
	int i=0;
	if (attrs)
		while (attrs[i]!=0)
		{
			attMap[(char *)(attrs[i])] = (char *)(attrs[i+1]);
			i+=2;
			//printf("%s %s %s\n",name,attrs[i],attrs[i+1]);
		}

	((KD_XMLParser*)user_data)->ListenStartElement((char *)name, attMap);
}

//void endElement(void *user_data, const CHAR *name)
void endElement(void *user_data, const xmlChar *name)
{

	//printf("%s",name);
	((KD_XMLParser*)user_data)->ListenEndElement((char *)name);
}

void characters(void *user_data, const xmlChar *ch, int len)
{
	string temp;
	for (int i=0; i<len; i++)
	{
		temp[i]+=(char)ch[i];
	}
	((KD_XMLParser*)user_data)->ListenStringElement(temp);
}

int myXmlSAXParseFile(xmlSAXHandlerPtr sax, void *user_data, const char *filename)
{
    int ret = 0;
    xmlParserCtxtPtr ctxt;

    ctxt = xmlCreateFileParserCtxt(filename);
    if (ctxt == NULL) return -1;
    ctxt->sax = sax;
    ctxt->userData = user_data;

    xmlParseDocument(ctxt);

    if (ctxt->wellFormed)
        ret = 0;
    else
        ret = -1;
    if (sax != NULL)
        ctxt->sax = NULL;
    xmlFreeParserCtxt(ctxt);
    
    return ret;
}


bool KD_XMLParser::InitLibXML()
{
	libXMLSAXParser = new xmlSAXHandler();

	memset(libXMLSAXParser,0, sizeof(xmlSAXHandler));
	
	libXMLSAXParser->startElement = ::startElement;
	libXMLSAXParser->endElement = ::endElement;
	libXMLSAXParser->characters = ::characters;

	return true;
}

bool KD_XMLParser::closeLibXML()
{
	delete libXMLSAXParser;
	return true;
}

KD_XMLParser::KD_XMLParser()
{
	InitLibXML();
}

KD_XMLParser::KD_XMLParser(const KD_FilePath &xmlFileName)
{
	InitLibXML();
	load(xmlFileName);
}

KD_XMLParser::~KD_XMLParser()
{
	closeLibXML();
}

bool KD_XMLParser::load(const KD_FilePath &xmlFileName)
{
	this->xmlFileName = xmlFileName;
	return true;
}

bool KD_XMLParser::Parse()
{
	if (!xmlFileName.IsArchived())
	{
		//xmlDocPtr ret = xmlSAXParseFile(libXMLSAXParser, xmlFileName.GetFullPath().c_str(), false);
		myXmlSAXParseFile(libXMLSAXParser, this, xmlFileName.GetFullPath().c_str());

		return true;
	}

	return false;
}

/*void  KD_XMLParser::startDocument ()
{
//	printf("Start Document\n");
}

string XMLChToString(const XMLCh *const xmlStr)
{
	string str;
	
	for (int i=0; xmlStr[i]!=0; i++)
	{
		str+=(char)xmlStr[i];
	}
	return str;
}

void  KD_XMLParser::startElement (const XMLCh *const name,AttributeList &attributes)
{
	

	map<string,string> attStr;

	for (unsigned int i=0; i<attributes.GetLength(); i++)
		attStr[XMLChToString(attributes.getName(i))] = XMLChToString(attributes.getValue(i));

	ListenStartElement(XMLChToString(name), attStr);
}

void  KD_XMLParser::error (const SAXParseException &exception)
{
	char* message = XMLString::transcode(exception.getMessage());
	KD_LogFile::printf2("Recoverable error parsing file %s: %s\n",xmlFileName.GetFullPath().c_str(),message);
	//delete [] message;
}
 
void  KD_XMLParser::fatalError (const SAXParseException &exception)
{
	char* message = XMLString::transcode(exception.getMessage());
	KD_LogFile::printf2("Fatal error parsing file %s: %s\n",xmlFileName.GetFullPath().c_str(),message);
	//delete [] message;
}
  
void  KD_XMLParser::warning (const SAXParseException &exception)
{
	char* message = XMLString::transcode(exception.getMessage());
	KD_LogFile::printf2("Warning parsing file %s: %s\n",xmlFileName.GetFullPath().c_str(),message);
	//delete [] message;
}

void  KD_XMLParser::resetErrors ()
{
}

void  KD_XMLParser::characters (const XMLCh *const chars,const unsigned int length)
{
}
  
void  KD_XMLParser::endDocument ()
{
}
  
void  KD_XMLParser::endElement (const XMLCh *const name)
{
	ListenEndElement(XMLChToString(name));
}*/

void KD_XMLParser::ListenStartElement(string name, map<string,string> attributes)
{
	KD_LogFile::printf2("Start Element: %s\n",name.c_str());
}

void KD_XMLParser::ListenStringElement(string name)
{
	KD_LogFile::printf2("String Element: %s\n",name.c_str());
}

void KD_XMLParser::ListenEndElement(string name)
{
	KD_LogFile::printf2("End Element: %s\n",name.c_str());
}
