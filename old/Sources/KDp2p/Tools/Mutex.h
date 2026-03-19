#ifndef Mutex_H
#define Mutex_H

#include "defines.h"

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

/**
	A mutex.
*/
class DllExport KDp2p_Mutex
{
	SDL_mutex *mutex;	

public:
	KDp2p_Mutex();
	virtual ~KDp2p_Mutex();
	
	virtual void Lock();
	virtual void Unlock();
};

#endif
