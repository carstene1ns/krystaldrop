#ifndef Event_H
#define Event_H

#include "../Tools/defines.h"

class KD_EventManager;

/**
	An event. Once activated, it is called at each frame by the EventManager.
  */
class DllExport KD_Event
{
	friend class KD_EventManager;
protected:
	/**
		If true, Update and Display will be called on each frame.
		Else the event is left untouched.
	*/
	bool activate;

	/**
		If true, the timer will be updated on each frame.
		Else it won't.
		If the event is not activated, the timer is not whatever the value of activateTimer is.
	*/
	bool activateTimer;

	/**
		If true, the event will be destructed after next Update.
	*/
	bool autoDestruct;

	/**
		Time elapsed since the event has been activated.
		Time is expressed in seconds.
	*/
	float timeElapsed;

	/**
		If timeElapsed is greater than countDown, then the Event will autodestruct itself.
		If countDown<0, countDown is disabled.
	*/
	float countDown;

	/**
		The EventManager managing this event.
		Only one manager can manage an event at the same time.
	*/
	KD_EventManager *eventManagerOwner;

	/**
		Update the event if it is activated.
		It basically calls Update(timeElapsed);
	*/
	void UpdateEvent(float timeElapsedSinceLastFrame);

	/**
		Displays the event if it is activated.
		It basically calls Display();
	*/
	void DisplayEvent();

public:
	KD_Event();
	virtual ~KD_Event();

	/**
		Remove this event from the event manager.
	*/
	void RemoveFromEventManager();

	/**
		Update the event.
		This function should be overidden by the daughter class.
	*/
	virtual void Update(float timeElapsed);

	/**
		Display the event.
		This function should be overited by the daughter class.
	*/
	virtual void Display();

	/**
		Set the autodestruction flag of the event
	*/
	void AutoDestructEvent();

	/**
		Set the activate flag of the event
	*/
	void ActivateEvent();

	/**
		Pauses the event (sets the activate flag to false).
	*/
	void PauseEvent();

	/**
		Only pauses the timer. The event is still activated.
	*/
	void PauseTimer();

	/**
		Restarts the timer. The timer is effectively started only if the event is activated.
		If not, it will wait for next activation.
	*/
	void RestartTimer();

	/**
		Reset the timer of the event
	*/
	void ResetTimer();

	/**
		Sets the time after which the event will autodestruct itself.
	*/
	void SetCountDownTimer(float countDown);
};	

#endif
