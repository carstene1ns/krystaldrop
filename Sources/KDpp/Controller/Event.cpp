#include "Event.h"

#include "EventManager.h"

KD_Event::KD_Event()
{
	activate = false;
	activateTimer = true;
	autoDestruct = false;
	timeElapsed=0;
	countDown=-1;
}

KD_Event::~KD_Event()
{
	// valable car this est encore valable lors du delete!
	RemoveFromEventManager();
}

void KD_Event::RemoveFromEventManager()
{
	eventManagerOwner->DeleteEvent(this);
}

void KD_Event::UpdateEvent(float timeElapsedSinceLastFrame)
{
	if (!activate) return;

	if (activateTimer)
		timeElapsed += timeElapsedSinceLastFrame;

	if (countDown>=0 && timeElapsed>countDown)
	{
		AutoDestructEvent();
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

void KD_Event::AutoDestructEvent()
{
	autoDestruct = true;
}

void KD_Event::ActivateEvent()
{
	activate=true;
}

void KD_Event::PauseEvent()
{
	activate=false;
}

void KD_Event::PauseTimer()
{
	activateTimer = false;
}

void KD_Event::RestartTimer()
{
	activateTimer = true;
}

void KD_Event::ResetTimer()
{
	timeElapsed=0;
}

void KD_Event::SetCountDownTimer(float countDown)
{
	this->countDown = countDown;
}
