#include "Thread.h"

#include <assert.h>
#include "../../KDpp/Tools/Logfile.h"

int FnStartThread(void *myThread)
{
	assert(myThread);
	((KDp2p_Thread*)myThread)->Run();
	return 0;
}

KDp2p_Thread::KDp2p_Thread()
{
	thread = 0;
}

KDp2p_Thread::~KDp2p_Thread()
{

}

void KDp2p_Thread::Start()
{
	if (thread != 0)
	{
		KD_LogFile::printf2("Warning! Trying to start a thread already started!");
		return;
	}

	thread = SDL_CreateThread(&FnStartThread, this);
}

void KDp2p_Thread::Run()
{
}

void KDp2p_Thread::WaitThread()
{
	SDL_WaitThread(thread, NULL);
}
