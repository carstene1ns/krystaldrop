#include "Socket.h"

KDp2p_Socket::KDp2p_Socket()
{
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	address.setIP(INADDR_ANY);
	address.setPort(0);

}


KDp2p_Socket::~KDp2p_Socket()
{
	Close();
}

bool KDp2p_Socket::Connect(const KDp2p_NetworkAddress &_address)
{
	address = _address;
	int ret = connect(socketFd, (sockaddr*)&address.myAddr, sizeof(sockaddr));
	if (ret == 0)
		return true;
	else
		return false;
}

void KDp2p_Socket::Listen(unsigned short int port)
{
	address.setIP(INADDR_ANY);
	address.setPort(htons(port));
	bind(socketFd, (sockaddr*)&address.myAddr, sizeof (sockaddr));
	listen(socketFd, KDP2P_LISTEN_MAXPENDING_CONNECTIONS);
}

void KDp2p_Socket::Close()
{
#ifndef WIN32
	close(socketFd);
#else
	closesocket(socketFd);
#endif
}
