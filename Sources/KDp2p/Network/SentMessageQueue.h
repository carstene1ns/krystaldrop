#ifndef SentMessageQueue_H
#define SentMessageQueue_H

#include "../Tools/defines.h"
#include "../Tools/Mutex.h"
#include "../Tools/Thread.h"
#include "NetworkAddress.h"

#include <deque>
using namespace std;

class KDp2p_Message;
class KDp2p_UDPSocket;

/**
	The message queue containing messages to send.
*/
class DllExport KDp2p_SentMessageQueue : public KDp2p_Thread
{
	/*class AddressedMessage
	{
	public:
		KDp2p_Message *message;
		KDp2p_NetworkAddress address;

		AddressedMessage(KDp2p_Message *message, KDp2p_NetworkAddress address);
		/// When destroyed, the message is deleted.
		~AddressedMessage();

		AddressedMessage(const KDp2p_SentMessageQueue::AddressedMessage &that);

		AddressedMessage& operator = (const KDp2p_SentMessageQueue::AddressedMessage& that);
	};*/

	KDp2p_Mutex queueMutex;

	//deque<AddressedMessage*> messages;
	deque<KDp2p_Message*> messages;

	KDp2p_UDPSocket *socket;

	bool destroyThread;

public:
	/// Creates an empty queue using socket sock
	KDp2p_SentMessageQueue(KDp2p_UDPSocket *socket);

	/// Deletes the message queue
	virtual ~KDp2p_SentMessageQueue();

	/// Adds a message to the queue
	void AddMessage(KDp2p_Message *message);

	/// Starts the thread
	virtual void Run();
};

#endif
