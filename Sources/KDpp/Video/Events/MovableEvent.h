#ifndef MovableEvent_H
#define MovableEvent_H

#include "../../Tools/defines.h"

#include "../../Controller/Event.h"


/**
	An abstract event implemnting behaviour for movable events.
	This event will be subclassed into KD_TextEvent and KD_SpriteEvent.
	It can move linearly, quadratically, with rotating, zooming, alphatransparency and color shifting.	  
	Finally it can blink (if implemented in the child class)!

	When subclassing the KD_MovableEvent class the virtual methods Display and Update must NOT be overriten.
	Instead, KD_MovableEvent provides 2 new method to overrite: DisplayMovable and UpdateMovable
  */
class DllExport KD_MovableEvent : public KD_Event
{
protected:
	int x,y;
	int red,green,blue,alpha;
	float resizeX, resizeY, angle;

	unsigned char movingType;
	float movingParameters[27];

	/**
		When blinking, number of ticks during which the text is visible
	*/
	unsigned int timeVisible;

	/**
		When blinking, number of ticks during which the text is not visible
	*/
	unsigned int timeInvisible;

public:
	KD_MovableEvent();
	virtual ~KD_MovableEvent();

	/**
		Sets the coordinates of the KD_MovableEvent
	*/
	void setCoordinates(int x, int y);

	/**
		Update the event.
		This function should NOT be overited by the daughter class.
	*/
	virtual void Update(float timeElapsed);

	/**
		Update the event.
		This function should be overited by the daughter class.
	*/
	virtual void UpdateMovable(float timeElapsed);

	/**
		Display all Displayable events
		This function should NOT be overited by the daughter class.
	*/
	virtual void Display();

	/**
		Display all Displayable events
		This function should be overited by the daughter class.
	*/
	virtual void DisplayMovable();

	/**
		Sets the movement to linear mode.
		The text will go from x0,y0 to x1,y1 in "time" seconds and then will disappear.
	*/
	void setLinearMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, float time);

	//{
	/**
		Sets the movement to quadratic mode.
		The text will go from x0,y0 to x1,y1 and the x2,y2 in "time" seconds and then will disappear.
	*/
	void setQuadraticMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, int x2, int y2, int alpha2, float time);

	void setQuadraticMove(int x0, int y0, int r0, int g0, int b0, int alpha0, float resizeX0, float resizeY0, float angle0,
						  int x1, int y1, int r1, int g1, int b1, int alpha1, float resizeX1, float resizeY1, float angle1,
						  int x2, int y2, int r2, int g2, int b2, int alpha2, float resizeX2, float resizeY2, float angle2, float time);
	//}

	/**
		Sets the movement for a parabolic move directed by gravity.
	*/
	void setGravityMove(int x, int y, float speedX, float speedY, float gravity, int r0, int g0, int b0, int alpha0, int r1, int g1, int b1, int alpha1, float angleSpeed, float time);

	/**
		Sets the blinking of the text.
		timeVisible and timeInvisible must be specified in seconds.
	*/
	void setBlinking(float timeVisible, float timeInvisible);
};	

#endif
