#ifndef ConnectionManager_H
#define ConnectionManager_H

#include "../Tools/defines.h"

#include "../Network/NetworkAddress.h"

#include "MessageHandler.h"

#include <deque>
using namespace std;

class KDp2p_ConnectionListener;
class KDp2p_Message;
class KDp2p_P2PEngine;

#define CORE_MESSAGEID ((((('C'<<24) + ('O'<<16) + ('R'<<8) + 'E'))))
#define COAC_MESSAGEID ((((('C'<<24) + ('O'<<16) + ('A'<<8) + 'C'))))
#define HELO_MESSAGEID ((((('H'<<24) + ('E'<<16) + ('L'<<8) + 'O'))))
#define STCO_MESSAGEID ((((('S'<<24) + ('T'<<16) + ('C'<<8) + 'O'))))

/**
	Manages connections by ensuring a distant computer is indeed running the P2P protocol.
	A connexion is only a way to know that a distant computer is on (and to ensure we know when it turns off).
	To establish a connection, we send a connection request. (CORE: Connection Request)
	The distant computer send a connection accepted message. (COAC: Connection Accepted)
	They send each other HELO messages to keep connections alive. (HELO)
	One can stop a connexion with a STOP CONNECTION message. (STCO: Stop Connection)
*/
class DllExport KDp2p_ConnectionManager : public KDp2p_MessageHandler

{
	class TimedAddress
	{
	public:
		KDp2p_NetworkAddress address;
		unsigned int time;
		KDp2p_ConnectionListener *listener;
	};

	/**
		The list of address we are waiting for the ACCEPTED CONNECTION message.
	*/
	deque<TimedAddress> addressTryingToConnectTo;

	/**
		The address we should send the HELO message to, with the time at which we should send the message
		The listener is never used here.
	*/
	deque<TimedAddress> addressToSend;

	/**
		The address we are waiting a message from, with the time after which the connection will be considered as closed and the listener will be called.
	*/
	deque<TimedAddress> lastReceivedFromAddress;

	/**
		Pointer to the engine used to send messages.
	*/
	KDp2p_P2PEngine *engine;

	void SendSTCOMessage(KDp2p_NetworkAddress *address);

public:
	KDp2p_ConnectionManager(KDp2p_P2PEngine *engine);
	virtual ~KDp2p_ConnectionManager();

	/**
		Asks a computer for a connection
	*/
	void AddConnection(KDp2p_NetworkAddress *address, KDp2p_ConnectionListener *listener);

	/**
		Sends a HELO message to all computers needing it to keep the connexion alive.
	*/
	void SendUpdateMessages();

	/**
		Closes the given connection
	*/
	void CloseConnection(KDp2p_NetworkAddress *address);

	/**
		Closes all connections
	*/
	void CloseAllConnections();

	/**
		Analyses the received Connection Request and process it:
			Adds the sender to the connected address lists
			Sends a COAC message
	*/
	void RecvCOREMessage(KDp2p_Message *message);

	/**
		Analyses the received Connection Accepted message and process it:
			Removes the sender of the waiting connection address list
			Adds the sender to the connected address lists
	*/
	void RecvCOACMessage(KDp2p_Message *message);

	/**
		Analyzes the received HELO message.
	*/
	void RecvHELOMessage(KDp2p_Message *message);

	/**
		Analyzes the received STCO message.
	*/
	void RecvSTCOMessage(KDp2p_Message *message);

	/**
		Updates timeout by discarding invalid connections
	*/
	void ComputeTimeOut();

	/**
		Receives the message and sends it to the class
	*/
	virtual void HandleMessage(KDp2p_Message *message, int id);
};

#endif
