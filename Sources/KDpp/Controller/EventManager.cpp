#include "EventManager.h"

#include <assert.h>

#include "Event.h"
#include "../Video/Display.h"

KD_EventManager::KD_EventManager()
{
}

KD_EventManager::~KD_EventManager()
{
	for (unsigned int i=0; i<events.size(); i++)
		delete events[i];
}

void KD_EventManager::AddEvent(KD_Event *event)
{
	events.push_back(event);
	event->eventManagerOwner = this;
}

void KD_EventManager::DeleteEvent(KD_Event *event)
{
	unsigned int size= (unsigned int) events.size();
	
	unsigned int i;
	for (i= 0; i< size; i++)
		if (events[i]== event)
			break;

	assert(i!=size);

	events.erase(events.begin()+ i);
}

void KD_EventManager::UpdateEvents()
{
	for (int i=0; i<(int)events.size(); i++)
	{
		events[i]->UpdateEvent(Display::timeElapsed);
		if (events[i]->autoDestruct == true)
		{
			//events[i]->RemoveFromEventManager();		
			delete events[i];
			i--;
		}
	}
}

void KD_EventManager::DisplayEvents()
{
	for (unsigned int i=0; i<events.size(); i++)
		events[i]->DisplayEvent();
}

void KD_EventManager::DeleteAllEvents()
{
	while (events.size() != 0)
		events[0]->RemoveFromEventManager();
}

bool KD_EventManager::IsValid(KD_Event *event)
{
	unsigned int size= (unsigned int) events.size();
	
	unsigned int i;
	for (i= 0; i< size; i++)
		if (events[i]== event)
			break;

	if (i==size) return false;
	else return true;
}
