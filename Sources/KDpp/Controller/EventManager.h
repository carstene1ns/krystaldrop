#ifndef EventManager_H
#define EventManager_H

#include "../Tools/defines.h"

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "deque"

using namespace std;

class KD_Event;

/**
	The manager of Events.
	There is one event manager per controller.
  */
class DllExport KD_EventManager
{
protected:
	friend class KD_Event;

	deque<KD_Event *> events;

	KD_EventManager();
	virtual ~KD_EventManager();

	/**
		Delete the event of the deque of event known by KD_EventManager.
		This method is called by the destructor of KD_Event.
	*/
	void DeleteEvent(KD_Event *event);

	/**
		Tests if the given pointer is part of the deque of Events.
		This method is useful to know if a pointer is valid and if the given Event has not been autodestructed.
		Returns true if the pointer is valid, false otherwise.
	*/
	bool IsValid(KD_Event *event);

public:
	/**
		Adds the event to the deque of event known by KD_EventManager.
		This method is called by the constructor of KD_Event.
	*/
	void AddEvent(KD_Event *event);

	/**
		Update all events
	*/
	void UpdateEvents();

	/**
		Display all Displayable events
	*/
	void DisplayEvents();

	/**
		Delete all the events.
		This method can be useful for example when closing a controller.
	*/
	void DeleteAllEvents();
};

#endif
