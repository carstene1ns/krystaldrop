#ifndef SpriteEvent_H
#define SpriteEvent_H

#include "../../Tools/defines.h"

#include "MovableEvent.h"

//class KD_SpriteInstance;
//class KD_Sprite;
class KD_DisplayableInstance;
class KD_DisplayableResource;

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It Displays a specified text with the given font at the given position.
	It can also Display the time elapsed since it has been running (activateTextTimer).
	Finally it can blink!
  */
class DllExport KD_SpriteEvent : public KD_MovableEvent
{
	//KD_SpriteInstance *spr;
	KD_DisplayableInstance *spr;
	KD_DisplayableResource *sprResource;
	unsigned char DisplayFrom;

public:
	KD_SpriteEvent();
	virtual ~KD_SpriteEvent();

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
		Displays the sprite from top left coordinates
	*/
	//void DisplayFromTopLeft();

	/**
		Displays the sprite from center coordinates
	*/
	//void DisplayFromCenter();

	/**
		Displays the sprite from center coordinates on x axis, bottom coordinates on y axis.
		(Useful for characters).
	*/
	//void DisplayFromCenterBottom();

	/**
		Sets the sprite to Display
	*/
	void setSprite(KD_DisplayableResource *model);
};	

#endif
