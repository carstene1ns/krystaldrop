#ifndef InputBox_H
#define InputBox_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "string"

using namespace std;

#include "../interf/event.h"

class KD_Font;

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It displays a specified text with the given font at the given position.
	It can also display the time elapsed since it has been running (activateTextTimer).
  */
class KD_InputBox : public KD_Event
{
	string str;
	KD_Font *font;
	int x,y;
	unsigned char printFrom;
	int maxLength;

public:
	KD_InputBox();
	virtual ~KD_InputBox();

	/**
		Update the event.
		This function should be overited by the daughter class.
	*/
	virtual void Update(float timeElapsed);

	/**
		Display all displayable events
	*/
	virtual void Display();

	/**
		Set the coordinates of the text.
		These coordinates are modified by printFromLeft/printFromCenter/printFromRight.
	*/
	void setTextCoordinates(int x, int y);

	/**
		Presets the text to be displayed.
	*/
	void setText(char *str, ...);

	/**
		Returns the text that has been inputed.
	*/
	string &getText();

	/**
		Sets the font of the text to be displayed.
	*/
	void setTextFont(KD_Font *font);

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
		Sets the maximum number of character that are allowed to be inputed.
	*/
	void setMaxLength(int maxLength);

	/**
		Return the number of characters in the current input.
	*/
	int getLength();
};	

#endif
