#include "PingCounter.h"

KDp2p_PingCounter::KDp2p_PingCounter() : counter(0)
{
	it = pingedAddresses.begin();
}

KDp2p_PingCounter::~KDp2p_PingCounter()
{
	pingedAddresses.clear();
}

void KDp2p_PingCounter::OnQuestion()
{
	counter++;
}

void KDp2p_PingCounter::OnTimeOut()
{
	counter--;
}

void KDp2p_PingCounter::OnAnswer(KDp2p_NetworkAddress *address)
{
	counter--;
	mutex.Lock();
	pingedAddresses.push_back(*address);
	mutex.Unlock();
	if (pingedAddresses.size() == 1)
		it = pingedAddresses.begin();
}

int KDp2p_PingCounter::GetPingCounter()
{
	return counter;
}

KDp2p_NetworkAddress KDp2p_PingCounter::GetAddress()
{
	return *it;
}

bool KDp2p_PingCounter::NextAddress()
{
	it++;
	if (it == pingedAddresses.end())
		return false;
	else
		return true;
}

void KDp2p_PingCounter::ResetAddressesPointer()
{
	it = pingedAddresses.begin();
}

int KDp2p_PingCounter::GetNbPingAnswers()
{
	return pingedAddresses.size();
}