#include "eventmanager.h"

#include <assert.h>

#include "event.h"
#include "../video/Display.h"

KD_EventManager *KD_EventManager::singleton = 0;

KD_EventManager::KD_EventManager()
{
}

KD_EventManager::~KD_EventManager()
{
	for (unsigned int i=0; i<events.size(); i++)
		delete events[i];
}

KD_EventManager *KD_EventManager::getEventManager()
{
	return singleton;
}

KD_EventManager *KD_EventManager::initEventManager()
{
	if (singleton==0)
		singleton = new KD_EventManager();
	return singleton;
}

bool KD_EventManager::closeEventManager()
{
	if (!singleton) return false;
	delete singleton;
	singleton = 0;
	return true;
}

void KD_EventManager::addEvent(KD_Event *event)
{
	events.push_back(event);
}


void KD_EventManager::deleteEvent(KD_Event *event)
{
	unsigned int size= events.size();
	
	unsigned int i;
	for (i= 0; i< size; i++)
		if (events[i]== event)
			break;

	assert(i!=size);

	events.erase(events.begin()+ i);
}

void KD_EventManager::UpdateEvents()
{
	for (unsigned int i=0; i<events.size(); i++)
	{
		events[i]->UpdateEvent(Display::timeElapsed);
		if (events[i]->autoDestruct == true)
		{
			delete events[i];
			events[i]->RemoveFromEventManager();
			i--;
		}
	}
}

void KD_EventManager::DisplayEvents()
{
	for (unsigned int i=0; i<events.size(); i++)
		events[i]->DisplayEvent();
}

void KD_EventManager::deleteAllEvents()
{
	while (events.size() != 0)
		events[0]->RemoveFromEventManager();
}

bool KD_EventManager::isValid(KD_Event *event)
{
	unsigned int size= events.size();
	
	unsigned int i;
	for (i= 0; i< size; i++)
		if (events[i]== event)
			break;

	if (i==size) return false;
	else return true;
}
