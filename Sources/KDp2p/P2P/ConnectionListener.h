#ifndef ConnectionListener_H
#define ConnectionListener_H

#include "../Tools/defines.h"

#include "../Network/NetworkAddress.h"

/**
	Virtual class containing callbacks that will be called when a connection failure occurs
*/
class DllExport KDp2p_ConnectionListener
{
public:
	KDp2p_ConnectionListener();
	virtual ~KDp2p_ConnectionListener();

	virtual void ConnectionFailed(KDp2p_NetworkAddress address) {};

	virtual void ConnectionInterrupted(KDp2p_NetworkAddress address) {};

	virtual void ConnectionSucceeded(KDp2p_NetworkAddress address) {};

	virtual void ConnectionClosedByPeer(KDp2p_NetworkAddress address) {};
};

#endif
