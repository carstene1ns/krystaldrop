#ifndef EventManager_H
#define EventManager_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "deque"

using namespace std;

class KD_Event;

/**
	The manager of Events.
  */
class KD_EventManager
{
	friend class KD_Event;

	static KD_EventManager *singleton;
	
	deque<KD_Event *> events;

	KD_EventManager();
	virtual ~KD_EventManager();

	/**
		Adds the event to the deque of event known by KD_EventManager.
		This method is called by the constructor of KD_Event.
	*/
	void addEvent(KD_Event *event);

	/**
		Delete the event of the deque of event known by KD_EventManager.
		This method is called by the destructor of KD_Event.
	*/
	void deleteEvent(KD_Event *event);

public:
	/**
		Static function returning the EventManager.
	*/
	static KD_EventManager *getEventManager();

	/**
		Instanciate the singleton
	*/
	static KD_EventManager *initEventManager();

	/**
		Delete the singleton
	*/
	static bool closeEventManager();

	/**
		Update all events
	*/
	void UpdateEvents();

	/**
		Display all displayable events
	*/
	void DisplayEvents();

	/**
		Delete all the events.
		This method can be useful for example when closing a controller.
	*/
	void deleteAllEvents();

	/**
		Tests if the given pointer is part of the deque of Events.
		This method is useful to know if a pointer is valid and if the given Event has not been autodestructed.
		Returns true if the pointer is valid, false otherwise.
	*/
	bool isValid(KD_Event *event);

};

#endif
