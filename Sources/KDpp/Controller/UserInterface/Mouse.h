#ifndef Mouse_H
#define Mouse_H

#include "SDL/SDL.h"

#include "../../Tools/defines.h"

#define KD_DOUBLECLICK -1

/**
	A class to retrieve the state of the Mouse.
  */
class DllExport KD_Mouse
{
	static KD_Mouse *singleton;

	/// Coordinates of the mouse
	int x,y;

	/// Coordinates of the last click (button down)
	int xLastClick, yLastClick;

	/** 
		state of the buttons
		Usually, button 1 is the left button and 2 the right button
		the other 254 positions are for other buttons ;)
	*/
	bool buttons[256];

	/**
		true if the mouse had moved since last click, false otherwise
		Used for te double click test
	*/
	bool hasMovedSinceLastClick;

	/**
		Time of the last left click
	*/
	unsigned int lastClickTime;

	/**
		Sensitivity of the double click, in milliseconds 
		(number of milliseconds to wait before considering the double click is in fact 2 single clicks).
		Default is 500 ms.
	*/
	unsigned int doubleClickSensitivity;

	/**
		if true, the double click detection is disabled.
		Default is false.
	*/
	bool doubleClickDisabled;

	KD_Mouse();
	virtual ~KD_Mouse();

public:
	/**
		Static function returning the Keyboard.
	*/
	static KD_Mouse *GetMouse();

	/**
		Instanciate the singleton
	*/
	static KD_Mouse *InitMouse();

	/**
		Delete the singleton
	*/
	static bool CloseMouse();

	/**
		Update the position of the mouse, sets hasMovedSinceLastClick to true
	*/
	void UpdateMousePosition(const SDL_Event &event);
	
	//{
	/**
		Update the buttons position.
		Returns te number of the button pressed, or KD_DOUBLECLICK if there is a double click.
	*/
	int onButtonDown(const SDL_Event &event);
	int onButtonUp(const SDL_Event &event);
	//}

	void DisableDoubleClick();
	void EnableDoubleClick();

	/**
		Returns the coordinates of the last left button down event.
	*/
	void GetLastClickCoordinates(int &xLastClick, int &yLastClick);

	/**
		Returns the coordinates of the mouse
	*/
	void GetCoordinates(int &x, int &y);

	/**
		Return the state of button "nb"
	*/
	bool GetButtonState(int nb) const;
};

#endif
