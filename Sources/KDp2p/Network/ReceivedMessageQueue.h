#ifndef ReceivedMessageQueue_H
#define ReceivedMessageQueue_H

#include "../Tools/defines.h"
#include "../Tools/Mutex.h"
#include "../Tools/Thread.h"
#include "NetworkAddress.h"

#include <deque>
using namespace std;

class KDp2p_Message;
class KDp2p_UDPSocket;
class KDp2p_Packet;

/**
	The message queue containing messages to send.
*/
class DllExport KDp2p_ReceivedMessageQueue : public KDp2p_Thread
{
	KDp2p_Mutex queueMutex;

	/// Messages already received that can be retrieved
	deque<KDp2p_Message*> messagesReceived;

	/// Messages being received but not complete yet (packets missing)
	deque<KDp2p_Message*> messagesReceiving;

	KDp2p_UDPSocket *socket;

	bool destroyThread;

public:
	/// Creates an empty queue using socket sock
	KDp2p_ReceivedMessageQueue(KDp2p_UDPSocket *socket);

	/// Deletes the message queue
	virtual ~KDp2p_ReceivedMessageQueue();

	/// Starts the thread
	virtual void Run();

	/// Adds a packet to the class
	void InsertPacket(KDp2p_Packet *packet);

	/// Discard old messages
	void DiscardOldMessages();

	/**
		Picks a message from the queue.
		The message should be deleted by the user!
		Returns NULL if no messages are available
	*/
	 KDp2p_Message *PeekMessage();

};

#endif
