#ifndef EditField_H
#define EditField_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"

#include <string>
using namespace std;

#include "Widget.h"
#include "KeyboardCursor.h"

class KD_Image;
class KD_Font;

#define KD_EDITFIELD_NORMAL			0
#define KD_EDITFIELD_OVER			1
#define KD_EDITFIELD_SELECTED		2

/**
	A button (a rectangular widget sending messages when clicked upon and containing a text)
  */
class DllExport KD_EditField : public KD_Widget, KD_KeyboardCursor
{
	//{
	/**
		Images of the editfield:
		0 = normal
		1 = the mouse is over
		2 = the cursor is active
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
		Image of the cursor
	*/
	KD_Image *cursor;

	/**
		Font of the checkbox
	*/
	KD_Font *font;

	/**
		Name of the fonts of the button
	*/
	string fontName;

	/// Text to be Displayed
	string text;
	
	/// position of the cursor
	int cursorPosition;

	/// position of the cursor in pixel from xWidget
	int cursorPixelPosition;

	/// One of the 3 editfield states: KD_EDITFIELD_NORMAL, KD_EDITFIELD_OVER, KD_EDITFIELD_SELECTED
	int editfieldState;

	/// Action number when the return key is pressed
	int actionOnReturn;

	/// Action number when the editfield is unselected
	int actionOnUnselect;

	/// Maximum number of characters in the field, default is 100
	unsigned int maxChars;

public:
	/**
		Constructor setting the owner
	*/
	KD_EditField(int x, int y, int width, int height);
	
	/**
		Destructor
	*/
	virtual ~KD_EditField();

	/**
		Release from the ResourceSet of the controller the previously needed resources.
	*/
	virtual void deleteEditField();

	/**
		Set the text to be Displayed
	*/
	virtual void SetText(const string &text);

	/**
		Returns the text Displayed
	*/
	virtual string GetText();

	/**
		Set the font of the button.
		If the string is "", the associated font is not changed.
	*/
	virtual void SetFont(const string &fontName);

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

	/**
		releases the cursor (sets the editfield in non selected mode).
	*/
	virtual void ReleaseCursor();

	/**
		Method called when a key is pressed
	*/
	virtual bool onWidgetKeyDown(SDLKey key);

	/**
		Sets the maximum number of characters in the field.
	*/
	void setMaxChars(int maxChars);

	/**
		Returns the maximum number of characters in the field.
	*/
	int getMaxChars() const;
};

#endif
