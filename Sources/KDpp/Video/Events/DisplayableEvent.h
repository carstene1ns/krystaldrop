#ifndef SpriteEvent_H
#define SpriteEvent_H

#include "../../Tools/defines.h"
#include "MovableEvent.h"

class KD_DisplayableInstance;
class KD_DisplayableResource;

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It Displays a specified text with the given font at the given position.
	It can also Display the time elapsed since it has been running (activateTextTimer).
	Finally it can blink!
  */
class  DllExport KD_DisplayableEvent : public KD_MovableEvent
{
	KD_DisplayableInstance *spr;
	KD_DisplayableResource *model;

public:
	KD_DisplayableEvent();
	virtual ~KD_DisplayableEvent();

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
		Sets the sprite to Display
	*/
	void setDisplayableResource(KD_DisplayableResource *model);
};	

#endif
