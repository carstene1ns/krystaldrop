#ifndef Widget_H
#define Widget_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"

class KD_WidgetController;

/**
	Base class for any user interface components.
  */
class DllExport KD_Widget
{
	friend class KD_WidgetContainer;
protected:
	/// Coordinates of the Widget
	int xWidget,yWidget,widthWidget,heightWidget;

	/// Alpha blending of the widget (between 0 and 255)
	int alphaWidget;

	/// Controller owning the Widget
	KD_WidgetController *ownerController;

	/**
		Default Constructor
	*/
	KD_Widget();

	/**
		Constructor setting the owner
	*/
	//KD_Widget(KD_WidgetController *_ownerController);

public:
	/**
		Destructor
	*/
	virtual ~KD_Widget();

	/**
		Method called when a key is pressed
	*/
	virtual bool onWidgetKeyDown(SDLKey key);
	
	/**
		Method called when a key is released
	*/
	virtual bool onWidgetKeyUp(SDLKey key);

	/**
		Method called when the cursor moves.
		This is forwarded to the widget only if the cursor is inside the widget OR was inside the widget last mousemotionevent.
	*/
	virtual bool onWidGetMouseMotion(int x, int y, int xPrevious, int yPrevious);

	/**
		Method called when the cursor enters the Widget
	*/
	virtual bool onMouseEnter();

	/**
		Method called when the cursor leaves the Widget
	*/
	virtual bool onMouseLeave();

	/**
		Method called when a click is performed in the widget.
		A click is performed if there is a release button event IN the widget and if the previous press button event was also performed in the widget.
	*/
	virtual bool onWidgetClick(int button);

	/**
		Method called when a button is pressed inside the Widget
	*/
	virtual bool onWidGetMouseButtonDown(int button, int x, int y);

	/**
		Method called when a button is released in the Widget
		x and y are the coordinates where the button was released
		xLastClick and yLastClick are the coordinates where the button was pressed
	*/
	virtual bool onWidGetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick);

	/**
		Method called when enabling the controller containing the widget.
	*/
	virtual void OnEnableWidget();

	/**
		Method called when disabling the controller containing the widget.
	*/
	virtual void OnDisableWidget();

	/**
		Returns the position of the widget
	*/
	virtual void GetWidgetPosition(int &x, int &y, int &width, int &height);

	/**
		Returns true if the coordinates are inside the widget, false otherwise.
	*/
	virtual bool isInside(int x, int y) const;

	/**
		Method called each frame
	*/
	virtual void PerformWidget();

	/**
		Method called each frame to Display the widget
	*/
	virtual void DisplayWidget(int alpha);

	/**
		Sets the alpha transparency of the widget
	*/
	virtual void SetAlpha(int alpha);

};

#endif
