#ifndef TextEvent_H
#define TextEvent_H

#include "../interf/event.h"


class KD_Font;

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It displays a specified text with the given font at the given position.
	It can also display the time elapsed since it has been running (activateTextTimer).
  */
class KD_TextEvent : public KD_Event
{
	char buf[1000];
	KD_Font *font;
	int x,y;
	int alpha;
	unsigned char printFrom;

	unsigned char movingType;
	float movingParameters[9];


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
	virtual void Update(float timeElapsed);

	/**
		Display all displayable events
	*/
	virtual void Display();

	void setTextCoordinates(int x, int y);

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

	/**
		Sets the movement to linear mode.
		The text will go from x0,y0 to x1,y1 in "time" seconds and then will disappear.
	*/
	void setLinearMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, float time);

	/**
		Sets the movement to quadratic mode.
		The text will go from x0,y0 to x1,y1 and the x2,y2 in "time" seconds and then will disappear.
	*/
	void setQuadraticMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, int x2, int y2, int alpha2, float time);
};	

#endif
