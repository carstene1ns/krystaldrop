#ifndef Thread_H
#define Thread_H

#include "defines.h"

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"


/**
	A thread.
*/
class DllExport KDp2p_Thread
{
	SDL_Thread *thread;	

protected:
	KDp2p_Thread();
	virtual ~KDp2p_Thread();

public:
	
	virtual void Start();

	/**
		This function should be overriden to implement the functionnality of the thread.
	*/
	virtual void Run();

	/**
		Waits for the thread to finish (blocking call)
	*/
	virtual void WaitThread();
};

#endif
