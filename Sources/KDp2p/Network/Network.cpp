#include "Network.h"

#include "../Tools/NetException.h"

#include <assert.h>
#include <SDL/SDL_net.h>

KDp2p_Network *KDp2p_Network::singleton = 0;

KDp2p_Network::KDp2p_Network()
{
	int res = SDLNet_Init();
	if (res == -1)
		throw KDp2p_NetException(SDLNet_GetError());

	// A changer éventuellement!
	packetMaxSize = 4096;
	receiveTimeOut = 5000;
}

KDp2p_Network::~KDp2p_Network()
{
	SDLNet_Quit();
}

KDp2p_Network *KDp2p_Network::initNetwork()
{
	/*if (singleton)
		closeNetwork();*/
	if (singleton)
		return singleton;
	
	singleton = new KDp2p_Network();
	return singleton;
}

void KDp2p_Network::closeNetwork()
{
	////////// I SHOULD PUT A REFERENCE COUNTER HERE....
	if (!singleton)
		return;
	
	delete singleton;
	singleton = 0;
}

KDp2p_Network *KDp2p_Network::getNetwork()
{
	assert(singleton);
	return singleton;
}

void KDp2p_Network::setPacketMaxSize(int size)
{
	packetMaxSize = size;
}

int KDp2p_Network::getPacketMaxSize()
{
	return packetMaxSize;
}

void KDp2p_Network::setReceiveTimeOut(int timeOut)
{
	receiveTimeOut = timeOut;
}

int KDp2p_Network::getReceiveTimeOut()
{
	return receiveTimeOut;
}
