#ifndef InputBox_H
#define InputBox_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "string"

using namespace std;

#include "../../KDpp/Controller/Event.h"

class KD_Font;

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It Displays a specified text with the given font at the given position.
	It can also Display the time elapsed since it has been running (activateTextTimer).
  */
class KD_InputBox : public KD_Event
{
	string str;
	KD_Font *font;
	int x,y;
	unsigned char PrintFrom;
	unsigned short maxLength;

public:
	KD_InputBox();
	virtual ~KD_InputBox();

	/**
		Update the event.
		This function should be overited by the daughter class.
	*/
	virtual void Update(float timeElapsed);

	/**
		Display all Displayable events
	*/
	virtual void Display();

	/**
		Set the coordinates of the text.
		These coordinates are modified by PrintFromLeft/PrintFromCenter/PrintFromRight.
	*/
	void SetTextCoordinates(int x, int y);

	/**
		Presets the text to be Displayed.
	*/
	void SetText(char *str, ...);

	/**
		Returns the text that has been inputed.
	*/
	string &GetText();

	/**
		Sets the font of the text to be Displayed.
	*/
	void SetTextFont(KD_Font *font);

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

	/**
		Sets the maximum number of character that are allowed to be inputed.
	*/
	void SetMaxLength(unsigned short maxLength);

	/**
		Return the number of characters in the current input.
	*/
	int GetLength();
};	

#endif
