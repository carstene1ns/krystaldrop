#ifndef KeyboardCursor_H
#define KeyboardCursor_H

#include "SDL/SDL.h"

#include "../../Tools/defines.h"


/**
	Any class using a text cursor should inherit from this class.
	There is only one active text cursor at a time in an application, and this class tracks it with a static member.
	When a cursor is grabed by a member, the function ReleaseCursor is called for the previous owner of the cursor.
  */
class DllExport KD_KeyboardCursor
{
	/// The current owner of the text cursor
	static KD_KeyboardCursor *currentCursor;

public:
	KD_KeyboardCursor();
	virtual ~KD_KeyboardCursor();
	
	/**
		Grabs the cursor.
		This will automatically call realeaseCursor.
	*/
	void GrabKeyboardCursor();

	/**
		Pure virtual function that should be overwritten by the class inheriting from KD_KeyboardCursor
	*/
	virtual void ReleaseCursor()=0;

	/**
		Returns if the instance is the owner of the text cursor
	*/
	bool IsOwningKeyboardCursor() const;

	/**
		Forces the current owner of the keyboard cursor to release it.
	*/
	static void ForceRelease();
};

#endif
