#ifndef CheckBox_H
#define CheckBox_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"

#include <string>
using namespace std;

#include "Widget.h"

class KD_Image;
class KD_Font;

#define KD_CHECKBOX_NORMAL			0
#define KD_CHECKBOX_CHECKED			1
#define KD_CHECKBOX_OVER			2
#define KD_CHECKBOX_OVER_CHECKED	3

/**
	A button (a rectangular widget sending messages when clicked upon and containing a text)
  */
class DllExport KD_CheckBox : public KD_Widget
{
	//{
	/**
		Images of the button:
		0 = not checked state
		1 = checked state
		2 = mouse is inside not checked state
		3 = mouse is inside checked state
	*/
	KD_Image *checkbox[4];
	//}

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
	
	/// One of the 4 checkbox states: KD_CHECKBOX_NORMAL, KD_CHECKBOX_CHECKED, KD_CHECKBOX_OVER, KD_CHECKBOX_OVER_CHECKED
	int checkboxState;

	/// Action number when the checkbox is checked
	int actionOnCheck;

	/// Action number when the checkbox is unchecked
	int actionOnUncheck;

public:
	/**
		Constructor setting the owner
	*/
	KD_CheckBox(int x, int y, string text);
	
	/**
		Destructor
	*/
	virtual ~KD_CheckBox();

	/**
		Release from the ResourceSet of the controller the previously needed resources.
	*/
	virtual void deleteCheckBox();

	/**
		Set the text to be Displayed
	*/
	virtual void SetText(const string &text);

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
		Returns whether the checkbox is checked or not
	*/
	virtual bool isChecked() const;

	/**
		Checks the checkbox.
		The check event is not sent to the controller
	*/
	virtual void check();

	/**
		Unchecks the checkbox.
		The uncheck event is not sent to the controller
	*/
	virtual void uncheck();

	/**
		Sets the number of the action sent to the controller when the checkbox is checked
	*/
	virtual void setOnCheck(int action);

	/**
		Sets the number of the action sent to the controller when the checkbox is unchecked
	*/
	virtual void setOnUncheck(int action);
};

#endif
