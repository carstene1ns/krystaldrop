#include "PingCounter.h"

KDp2p_PingCounter::KDp2p_PingCounter() : counter(0)
{
	
}

KDp2p_PingCounter::~KDp2p_PingCounter()
{

}

void KDp2p_PingCounter::OnQuestion()
{
	counter++;
}

void KDp2p_PingCounter::OnTimeOut()
{
	counter--;
}

void KDp2p_PingCounter::OnAnswer()
{
	counter--;
}

int KDp2p_PingCounter::getPingCounter()
{
	return counter;
}
