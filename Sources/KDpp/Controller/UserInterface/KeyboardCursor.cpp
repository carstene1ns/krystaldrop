#include "KeyboardCursor.h"

KD_KeyboardCursor *KD_KeyboardCursor::currentCursor=0;

KD_KeyboardCursor::KD_KeyboardCursor()
{

}

KD_KeyboardCursor::~KD_KeyboardCursor()
{

}

void KD_KeyboardCursor::GrabKeyboardCursor()
{
	if (currentCursor)
		currentCursor->ReleaseCursor();

	currentCursor = this;
}

bool KD_KeyboardCursor::IsOwningKeyboardCursor() const
{
	if (currentCursor == this)
		return true;
	else
		return false;
}

void KD_KeyboardCursor::ForceRelease()
{
	if (currentCursor)
		currentCursor->ReleaseCursor();

	currentCursor = 0;
}
