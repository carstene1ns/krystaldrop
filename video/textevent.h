#ifndef TextEvent_H
#define TextEvent_H

#include "font.h"
#include "movableevent.h"

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It displays a specified text with the given font at the given position.
	It can also display the time elapsed since it has been running (activateTextTimer).
	Finally it can blink!
  */
class KD_TextEvent : public KD_MovableEvent
{
	char buf[PRINTF_BUF_SIZE];
	KD_Font *font;
	unsigned char printFrom;

	/**
		True if the text to display is the current time.
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
		Display all displayable events
	*/
	virtual void DisplayMovable();

	/**
		Sets the text to be displayed.
	*/
	void setText(char *str, ...);

	/**
		Sets the font of the text to be displayed.
	*/
	void setTextFont(KD_Font *font);

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
	void printFromLeft();

	/**
		Prints the text from the center coordinates
	*/
	void printFromCenter();

	/**
		Prints the text from the right coordinates
	*/
	void printFromRight();

};	

#endif
