#ifndef WidgetContainer_H
#define WidgetContainer_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"
#include "Widget.h"

#include <string>
#include <map>
using namespace std;

class KD_WidgetController;

/**
	Base class for any user interface components.
  */
class DllExport KD_WidgetContainer : public KD_Widget
{
protected:
	/**
		List of widgets contained by this container
	*/
	map <string, KD_Widget *> widgets;

	/**
		Default Constructor
	*/
	KD_WidgetContainer();

	/**
		Constructor setting the owner
	*/
	//KD_WidgetContainer(KD_WidgetController *_ownerController);

public:
	/**
		Destructor
	*/
	virtual ~KD_WidgetContainer();

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
		x and y are expressed relative to the WidgetContainer containing them.
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
		x and y are expressed relative to the WidgetContainer containing them.
	*/
	virtual bool onWidGetMouseButtonDown(int button, int x, int y);

	/**
		Method called when a button is released in the Widget
		x and y are the coordinates where the button was released
		xLastClick and yLastClick are the coordinates where the button was pressed
		x and y and xLastClick and yLastClick are expressed relative to the WidgetContainer containing them.
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
		Method called each frame
	*/
	virtual void PerformWidget();

	/**
		Method called each frame to Display the widget
	*/
	virtual void DisplayWidget(int alpha);

	/**
		Adds a widget "widget" named "name" to the container
	*/
	virtual bool AddWidget(string name, KD_Widget *widget);

	/**
		Removes a widget named "name" from the container and deletes it.
	*/
	virtual bool RemoveWidget(string name);

	/**
		Returns the widget named "name".
	*/
	virtual KD_Widget *GetWidget(string name);

	/**
		Removes all widgets from the container and deletes them.
	*/
	virtual bool RemoveAll();
};

#endif
