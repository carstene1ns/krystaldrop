#ifndef SpriteEvent_H
#define SpriteEvent_H

#include "movableevent.h"

class KD_SpriteInstance;
class KD_Sprite;

/**
	A text event. Once activated, it is called at each frame by the EventManager.
	It displays a specified text with the given font at the given position.
	It can also display the time elapsed since it has been running (activateTextTimer).
	Finally it can blink!
  */
class KD_SpriteEvent : public KD_MovableEvent
{
	KD_SpriteInstance *spr;
	unsigned char displayFrom;

public:
	KD_SpriteEvent();
	virtual ~KD_SpriteEvent();

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
		Displays the sprite from top left coordinates
	*/
	void displayFromTopLeft();

	/**
		Displays the sprite from center coordinates
	*/
	void displayFromCenter();

	/**
		Displays the sprite from center coordinates on x axis, bottom coordinates on y axis.
		(Useful for characters).
	*/
	void displayFromCenterBottom();

	/**
		Sets the sprite to display
	*/
	void setSprite(KD_Sprite *model);
};	

#endif
