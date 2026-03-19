#ifndef UDPSocket_H
#define UDPSocket_H

#include "../Tools/defines.h"
#include <SDL/SDL_net.h>

/**
	An UDP socket, used for sending and receiving messages.
*/
class DllExport KDp2p_UDPSocket
{
	friend class KDp2p_Packet;

	UDPsocket udpsock;
	unsigned short int port;

	void Open();
	void Close();

public:
	/// Opens an UDP Socket on a random free port
	KDp2p_UDPSocket();
	/// Opens an UDP socket on the specified port
	KDp2p_UDPSocket(unsigned short int port);
	virtual ~KDp2p_UDPSocket();

	
	

};

#endif
