#ifndef Button_H
#define Button_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"

#include <string>
using namespace std;

#include "Widget.h"

class KD_Image;
class KD_Font;

#define KD_BUTTON_NORMAL		0
#define KD_BUTTON_MOUSE_OVER	1
#define KD_BUTTON_PRESSED		2

/**
	A button (a rectangular widget sending messages when clicked upon and containing a text)
  */
class DllExport KD_Button : public KD_Widget
{
	//{
	/**
		Images of the button:
		0 = in normal state
		1 = mouse is inside
		2 = button pressed
	*/
	KD_Image *background[3];
	KD_Image *top_border[3];
	KD_Image *bottom_border[3];
	KD_Image *left_border[3];
	KD_Image *right_border[3];
	KD_Image *topleft_corner[3];
	KD_Image *topright_corner[3];
	KD_Image *bottomleft_corner[3];
	KD_Image *bottomright_corner[3];
	//}

	/**
		Fonts of the button:
		0 = in normal state
		1 = mouse is inside
		2 = button pressed
	*/
	KD_Font *font[3];

	/**
		Name of the fonts of the button
	*/
	string fontName[3];

	/// Text to be Displayed
	string text;
	
	/// One of the 3 button states: KD_BUTTON_NORMAL, KD_BUTTON_MOUSE_OVER, KD_BUTTON_PRESSED
	int buttonState;

	/// Action number to be returned
	int actionNumber;

public:
	/**
		Constructor setting the owner
	*/
	KD_Button(int x, int y, int width, int height, string text, int actionNumber);
	
	/**
		Destructor
	*/
	virtual ~KD_Button();

	/**
		Release from the ResourceSet of the controller the previously needed resources.
	*/
	virtual void deleteButton();

	/**
		Set the text to be Displayed
	*/
	virtual void SetText(const string &text);

	/**
		Set the 3 fonts of the button.
		Input: the 3 names of the fonts in the controller.
		If a string is "", the associated font is not changed.
	*/
	virtual void SetFont(const string &fontNormal, const string &fontOver,const string &fontPressed);

	/**
		Method called each frame to Display the widget
	*/
	virtual void DisplayWidget(int alpha);

	/**
		Method called when the cursor enters the Widget
	*/
	virtual bool onMouseEnter();

	/**
		Method called when the cursor leaves the Widget
	*/
	virtual bool onMouseLeave();

	/**
		Method called when a button is pressed inside the Widget
		x and y are expressed relative to the WidgetContainer containing them.
	*/
	virtual bool onWidGetMouseButtonDown(int button, int x, int y);

	/**
		Method called when a button is released in the Widget
		x and y are the coordinates where the button was released
		xLastClick and yLastClick are the coordinates where the button was pressed
	*/
	virtual bool onWidGetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick);

	/**
		Method called when a click is performed in the widget.
		A click is performed if there is a release button event IN the widget and if the previous press button event was also performed in the widget.
	*/
	virtual bool onWidgetClick(int button);
};

#endif
