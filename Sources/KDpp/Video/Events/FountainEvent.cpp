//#include "../global.h"

#include <math.h>
#include <stdlib.h>

#include "../Display.h"
#include "FountainEvent.h"
#include "DisplayableEvent.h"
#include "../../Controller/EventManager.h"

KD_FountainEvent::KD_FountainEvent() : KD_Event()
{
//	x=SCR_HW;
//	y=SCR_H;

	x=0;
	y=0;

	xSpeed=0.0f;
	ySpeed=-300.0f;

	angle = 20.0f/180.0f*3.14f;

	powerVar = 0.2f; //20% of variation on power

	//rotationSpeedMax = 0.14f;
	rotationSpeedMax = 14.0f;

	gravity = 5.0f;

	spr=0;

	timeBetweenParticles = 10;

	r0=g0=b0=alpha0=r1=g1=b1=alpha1=255;
}

/*KD_FountainEvent::~KD_FountainEvent()
{
}*/

void KD_FountainEvent::Update(float timeElapsed)
{
	int nbPart = Display::GetTimeSlice(timeBetweenParticles);

	for (int i=0; i<nbPart; i++)
	{
		float theta = ((float)rand()/(float)RAND_MAX)*angle-angle/2.0f;
		float power = 1.0f + ((float)rand()/(float)RAND_MAX)*powerVar-powerVar/2.0f;
		float rotationSpeed = ((float)rand()/(float)RAND_MAX)*rotationSpeedMax*2.0f-rotationSpeedMax;
		float xS = xSpeed*power*(float)cos(theta)-ySpeed*(float)sin(theta);
		float yS = ySpeed*power*(float)cos(theta)+xSpeed*(float)sin(theta);
		KD_DisplayableEvent *part = new KD_DisplayableEvent();
		part->setDisplayableResource(spr);
		part->setGravityMove(x,y,xS,yS,gravity,r0,g0,b0,alpha0,r1,g1,b1,alpha1,rotationSpeed,3);
		part->ActivateEvent();
		eventManagerOwner->AddEvent(part);
	}
}

void KD_FountainEvent::Display()
{
}

void KD_FountainEvent::setCoordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}

void KD_FountainEvent::setTimeToLive(float time)
{
	SetCountDownTimer(time);
}

void KD_FountainEvent::setParticle(float xSpeed, float ySpeed, float angle, float powerVar, float gravity, KD_DisplayableResource *spr, int timeBetweenParticles)
{
	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;
	this->angle = angle;
	this->powerVar = powerVar;
	this->gravity = gravity;
	this->spr = spr;
	this->timeBetweenParticles = timeBetweenParticles;
}

void KD_FountainEvent::setParticleColors(int r0, int g0, int b0, int alpha0, int r1, int g1, int b1, int alpha1)
{
	this->r0 = r0;
	this->g0 = g0;
	this->b0 = b0;
	this->alpha0 = alpha0;
	this->r1 = r1;
	this->g1 = g1;
	this->b1 = b1;
	this->alpha1 = alpha1;
}
