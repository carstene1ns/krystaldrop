#include "event.h"

#include "eventmanager.h"

KD_Event::KD_Event()
{
	activate = false;
	autoDestruct = false;
	timeElapsed=0;
	countDown=-1;
	KD_EventManager::getEventManager()->addEvent(this);
}

KD_Event::~KD_Event()
{
	KD_EventManager::getEventManager()->deleteEvent(this);
}

void KD_Event::UpdateEvent(float timeElapsedSinceLastFrame)
{
	if (!activate) return;

	timeElapsed += timeElapsedSinceLastFrame;

	if (countDown>=0 && timeElapsed>countDown)
	{
		autoDestructEvent();
		return;
	}
	Update(timeElapsed);
}

void KD_Event::DisplayEvent()
{
	if (!activate) return;

	Display();
}

void KD_Event::Update(float timeElapsed)
{

}

void KD_Event::Display()
{
	
}

void KD_Event::autoDestructEvent()
{
	autoDestruct = true;
}

void KD_Event::activateEvent()
{
	activate=true;
}

void KD_Event::pauseEvent()
{
	activate=false;
}

void KD_Event::resetTimer()
{
	timeElapsed=0;
}

void KD_Event::setCountDownTimer(float countDown)
{
	this->countDown = countDown;
}