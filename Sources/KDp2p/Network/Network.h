#ifndef Network_H
#define Network_H

#include "../Tools/defines.h"

/**
	A singleton representing the network connexion.
*/
class DllExport KDp2p_Network
{
private:
	static KDp2p_Network *singleton;

	/// Maximum size for a packet
	int packetMaxSize;

	/**
		TimeOut before discarding a packet not complete
		default: 5000 ms
	*/
	int receiveTimeOut;

protected:
	KDp2p_Network();
	virtual ~KDp2p_Network();

public:
	static KDp2p_Network *initNetwork();
	static void closeNetwork();
	static KDp2p_Network *getNetwork();

	void setPacketMaxSize(int size);
	int getPacketMaxSize();

	void setReceiveTimeOut(int timeOut);
	int getReceiveTimeOut();

};

#endif
