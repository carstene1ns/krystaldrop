#ifndef Socket_H
#define Socket_H

#include "../Tools/defines.h"

#include "NetworkAddress.h"

/// Maximum number of connections queued and waiting to be accepted 
#define KDP2P_LISTEN_MAXPENDING_CONNECTIONS 100

/**
	A socket, used for sending and receiving messages.
*/
class DllExport KDp2p_Socket
{
//	friend class KDp2p_Packet;

	SOCKET socketFd;
	KDp2p_NetworkAddress address;

	bool Connect(const KDp2p_NetworkAddress &_address);
	void Listen(unsigned short int port);
	void Close();

public:
	/// Create a Socket to connect to a peer
	KDp2p_Socket();

	virtual ~KDp2p_Socket();

};

#endif
