#include "KeyboardCursor.h"

KD_KeyboardCursor *KD_KeyboardCursor::currentCursor=0;

KD_KeyboardCursor::KD_KeyboardCursor()
{

}

KD_KeyboardCursor::~KD_KeyboardCursor()
{

}

void KD_KeyboardCursor::grabKeyboardCursor()
{
	if (currentCursor)
		currentCursor->releaseCursor();

	currentCursor = this;
}

bool KD_KeyboardCursor::isOwningKeyboardCursor() const
{
	if (currentCursor == this)
		return true;
	else
		return false;
}

void KD_KeyboardCursor::forceRelease()
{
	if (currentCursor)
		currentCursor->releaseCursor();

	currentCursor = 0;
}
