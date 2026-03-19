#ifndef ConnectionManager_H
#define ConnectionManager_H

#include "../Tools/defines.h"

#include "../Network/NetworkAddress.h"
#include "ConnectionHandler.h"

#include "MessageHandler.h"

#include <deque>
#include <map>
using namespace std;

//class KDp2p_ConnectionListener;
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
public:
	class TimedAddress
	{
	public:
		KDp2p_NetworkAddress address;
		unsigned int time;
		//KDp2p_ConnectionListener *listener;
		deque<KDp2p_ConnectionHandler*> handlers;

		/// Removes a given connectionHandler. Returns true on success.
		bool RemoveConnectionHandler(KDp2p_ConnectionHandler *handler);
	};
private:

	/**
		The list of address we are waiting for the ACCEPTED CONNECTION message.
	*/
	deque<TimedAddress> addressTryingToConnectTo;


	/**
		The address we should send the HELO message to.
	*/
	deque<KDp2p_NetworkAddress> addressToSend;

	/**
		The time the last HELO was sent.
	*/
	int timeLastSent;

	/**
		The pos of the last Message Sent
		No iterators are used here because they are invalidated by erase()
	*/
	int posLastSent;

	/**
		The address we are waiting a message from, with the time after which the connection will be considered as closed and the listener will be called.
	*/
	deque<TimedAddress> lastReceivedFromAddress;

	/**
		Pointer to the engine used to send messages.
	*/
	KDp2p_P2PEngine *engine;

	void SendSTCOMessage(KDp2p_NetworkAddress *address);

	/**
		Returns an iterator to the TimedAddress in addressToSend.
		if "address" is not found, returns the end element of the iterator.
	*/
	deque<KDp2p_NetworkAddress>::iterator FindConnectionInAddressToSend(const KDp2p_NetworkAddress &address);

	/**
		Returns an iterator to the TimedAddress in addressTryingToConnectTo.
		if "address" is not found, returns the end element of the iterator.
	*/
	deque<TimedAddress>::iterator FindConnectionInAddressTryingToConnectTo(const KDp2p_NetworkAddress &address);

	/**
		Returns an iterator to the TimedAddress in lastReceivedFromAddress.
		if "address" is not found, returns the end element of the iterator.
	*/
	deque<TimedAddress>::iterator KDp2p_ConnectionManager::FindConnectionInLastReceivedFromAddress(const KDp2p_NetworkAddress &address);


public:
	KDp2p_ConnectionManager(KDp2p_P2PEngine *engine);
	virtual ~KDp2p_ConnectionManager();

	/**
		Asks a computer for a connection
	*/
	KDp2p_ConnectionHandler *AddConnection(const KDp2p_NetworkAddress &address, KDp2p_ConnectionListener *listener);

	/**
		Sends a HELO message to all computers needing it to keep the connexion alive.
	*/
	void SendUpdateMessages();

	/**
		Closes the given connection
		The handler won't be valid any more after the call to this method!
	*/
	void CloseConnection(KDp2p_ConnectionHandler *handler);

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
