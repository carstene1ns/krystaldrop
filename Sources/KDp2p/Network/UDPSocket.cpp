#include "UDPSocket.h"

#include "../Tools/NetException.h"

KDp2p_UDPSocket::KDp2p_UDPSocket()
{
	port = 0;
	Open();
}

KDp2p_UDPSocket::KDp2p_UDPSocket(unsigned short int port)
{
	this->port = port;
	Open();
}

KDp2p_UDPSocket::~KDp2p_UDPSocket()
{
	Close();
}

void KDp2p_UDPSocket::Open()
{
	udpsock = SDLNet_UDP_Open(port);
	if ((&udpsock) == 0)
		throw KDp2p_NetException("Unable to open requested port");
}

void KDp2p_UDPSocket::Close()
{
	SDLNet_UDP_Close(udpsock);
}
