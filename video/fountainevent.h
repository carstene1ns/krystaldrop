#ifndef FountainEvent_H
#define FountainEvent_H

#include "../interf/event.h"

class KD_Sprite;

/**
	An abstract event implemnting behaviour for movable events.
	This event will be subclassed into KD_TextEvent and KD_SprtieEvent.
	It can move linearly, quadratically, with rotating, zooming, alphatransparency and color shifting.	  
	Finally it can blink (if implemented in the child class)!

	When subclassing the KD_MovableEvent class the virtual methods Display and Update must NOT be overriten.
	Instead, KD_MovableEvent provides 2 new method to overrite: DisplayMovable and UpdateMovable
  */
class KD_FountainEvent : public KD_Event
{
protected:
	/// The center of the foutain
	int x,y;

	/// The initial speed of particles
	float xSpeed,ySpeed;

	/// The angle derivating from the initial speed
	float angle;

	/// The allowed deviation on power from the initial speed
	float powerVar;

	/// Maximum rotating speed (in rad/sec)
	float rotationSpeedMax;

	/// The gravity applying to particles
	float gravity;

	/// The sprite of the particles
	// TO DO: replace this with a deque of particles with probabilities, parameters, etc....
	KD_Sprite *spr;

	/// The time to wait between emitting 2 particles. In milliseconds.
	int timeBetweenParticles;

	/// initial and final color of particles
	int r0,g0,b0,alpha0,r1,g1,b1,alpha1;

public:
	KD_FountainEvent();
	virtual ~KD_FountainEvent();

	/**
		Update the event.
	*/
	virtual void Update(float timeElapsed);

	/**
		Display the event. A foutain does not displays itself... so the method will be empty.
	*/
	virtual void Display();

	/**
		Sets the coordinates of the foutain source
	*/
	void setCoordinates(int x, int y);

	/**
		Sets the time to live of the fountain (before auto destruction)
	*/
	void setTimeToLive(float time);

	/**
		Sets the coordinates of the foutain source
	*/
	void setParticle(float xSpeed, float ySpeed, float angle, float powerVar, float gravity, KD_Sprite *spr, int timeBetweenParticles);

	/**
		Sets the initial and final colors of particles
	*/
	void setParticleColors(int r0, int g0, int b0, int alpha0, int r1, int g1, int b1, int alpha1);
};	

#endif
