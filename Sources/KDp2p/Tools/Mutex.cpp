#include "Mutex.h"

#include "NetException.h"

KDp2p_Mutex::KDp2p_Mutex()
{
	mutex = SDL_CreateMutex();
}

KDp2p_Mutex::~KDp2p_Mutex()
{
	SDL_DestroyMutex(mutex);
}
	
void KDp2p_Mutex::Lock()
{
	int ret = SDL_LockMutex(mutex);
	if (ret == -1)
		throw KDp2p_NetException("Unable to lock mutex!");
}

void KDp2p_Mutex::Unlock()
{
	int ret = SDL_UnlockMutex(mutex);
	if (ret == -1)
		throw KDp2p_NetException("Unable to unlock mutex!");
}
