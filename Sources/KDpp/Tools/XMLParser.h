#ifndef XMLParser_H
#define XMLParser_H

#include <stdio.h>
#include <stdarg.h>
//#include <xercesc/Parsers/SAXParser.hpp>

#include "defines.h"

#include <string>
#include <map>
using namespace std;

//#include <xercesc/sax/HandlerBase.hpp>
#include <libxml/parser.h>

#include "FilePath.h"

/**
	Class used to read xml-files.
	It wraps the Xerces library
  */
class DllExport KD_XMLParser/* : public HandlerBase*/
{
	/// The Xerces SAX Parser.
//	SAXParser *xercesSAXParser;

	/// The Parser (libXML)
	xmlSAXHandlerPtr libXMLSAXParser;

//	bool InitXerces();
//	bool closeXerces();
	bool InitLibXML();
	bool closeLibXML();

protected:
	/// Name of the file to Parse
	KD_FilePath xmlFileName;

public:
	/**
		Default constructor.
	*/
	KD_XMLParser();

	/**
		Opens the XMLFile fileName.
	*/
	KD_XMLParser(const KD_FilePath &xmlFileName);

	/**
		Destructor, it frees the space allocated for the file.
	*/
	virtual ~KD_XMLParser();

	/**
		Opens the XMLFile named fileName.
	*/
	bool load(const KD_FilePath &xmlFileName);

	/**
		Parses the document
	*/
	bool Parse();

	/// Receive notification of the beginning of the document.
//	virtual void  startDocument ();
  
	/// Receive notification of the start of an element.
//	virtual void  startElement (const XMLCh *const name,AttributeList &attributes);

	///  Receive notification of a recoverable Parser error.
//	virtual void  error (const SAXParseException &exception);
 
	/// Report a fatal XML parsing error.	
//	virtual void  fatalError (const SAXParseException &exception);
  
	/// Receive notification of a Parser warning. More...
//	virtual void  warning (const SAXParseException &exception);
  
	/// Reset the Error handler object on its reuse.
//	virtual void  resetErrors ();

	/// Receive notification of character data.
//	virtual void  characters (const XMLCh *const chars,const unsigned int length);
  
	/// Receive notification of the end of a document.
//	virtual void  endDocument ();
  
	/// Receive notification of the end of an element.
//	virtual void  endElement (const XMLCh *const name);


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
