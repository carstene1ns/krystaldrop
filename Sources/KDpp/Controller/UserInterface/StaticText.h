#ifndef StaticText_H
#define StaticText_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"

#include <string>
using namespace std;

#include "Widget.h"

class KD_Font;

#define KD_ALIGN_LEFT 0
#define KD_ALIGN_CENTER 1
#define KD_ALIGN_RIGHT 2

/**
	A static text.
	This widget does not send any events.
  */
class DllExport KD_StaticText : public KD_Widget
{
	/**
		Font of the text
	*/
	KD_Font *font;

	/**
		Name of the font of the text
	*/
	string fontName;

	/// Text to be Displayed
	string text;

	/// text alignement
	char align;
	
public:
	/**
		Constructor setting the owner
	*/
	KD_StaticText(int x, int y, string text);
	
	/**
		Destructor
	*/
	virtual ~KD_StaticText();

	/**
		Release from the ResourceSet of the controller the previously needed resources.
	*/
	virtual void DeleteStaticText();

	/**
		Set the text to be Displayed
	*/
	virtual void SetText(const string &text);

	/**
		Set the 3 fonts of the button.
		Input: the 3 names of the fonts in the controller.
		If a string is "", the associated font is not changed.
	*/
	virtual void SetFont(const string &fontName);

	/**
		Sets the alignment
	*/
	virtual void SetAlignment(char align);

	/**
		Method called each frame to Display the widget
	*/
	virtual void DisplayWidget(int alpha);

};

#endif
