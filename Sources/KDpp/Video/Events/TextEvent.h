#ifndef TextEvent_H
#define TextEvent_H

#include "../../Tools/defines.h"

#include "../Font.h"
#include "MovableEvent.h"

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It Displays a specified text with the given font at the given position.
	It can also Display the time elapsed since it has been running (activateTextTimer).
	Finally it can blink!
  */
class DllExport KD_TextEvent : public KD_MovableEvent
{
	char buf[PRINTF_BUF_SIZE];
	KD_Font *font;
	unsigned char PrintFrom;

	/**
		True if the text to Display is the current time.
	*/
	bool isTextTimer;

public:
	KD_TextEvent();
	virtual ~KD_TextEvent();

	/**
		Update the event.
		This function should be overited by the daughter class.
	*/
	virtual void UpdateMovable(float timeElapsed);

	/**
		Display all Displayable events
	*/
	virtual void DisplayMovable();

	/**
		Sets the text to be Displayed.
	*/
	void SetText(char *str, ...);

	/**
		Sets the font of the text to be Displayed.
	*/
	void SetTextFont(KD_Font *font);

	/**
		transform the event into a timer that prints itself on each frame.
	*/
	void activateTextTimer();

	/**
		Stops the timer behaviour of KD_TextEnvent.
	*/
	void inactivateTextTimer();

	/**
		Prints the text from the left coordinates
	*/
	void PrintFromLeft();

	/**
		Prints the text from the center coordinates
	*/
	void PrintFromCenter();

	/**
		Prints the text from the right coordinates
	*/
	void PrintFromRight();

};	

#endif
