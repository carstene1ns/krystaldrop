#include "SentMessageQueue.h"

#include "UDPSocket.h"
#include "Message.h"

#include <SDL/SDL.h>

#include "../../KDpp/Tools/Logfile.h"

/*KDp2p_SentMessageQueue::AddressedMessage::AddressedMessage(KDp2p_Message *message, KDp2p_NetworkAddress address)
{
	this->message = message;
	this->address = address;
}

KDp2p_SentMessageQueue::AddressedMessage::~AddressedMessage()
{
	delete message;
}

KDp2p_SentMessageQueue::AddressedMessage::AddressedMessage(const AddressedMessage &that)
{
	this->message = that.message;
	this->address = that.address;	
}

KDp2p_SentMessageQueue::AddressedMessage& KDp2p_SentMessageQueue::AddressedMessage::operator = (const KDp2p_SentMessageQueue::AddressedMessage& that)
{
	if (&that != this)
	{
		this->AddressedMessage::~AddressedMessage();
		::new (this) AddressedMessage(that);
	}
	return *this;
}*/

////////////////////////////////////////////////////////////

KDp2p_SentMessageQueue::KDp2p_SentMessageQueue(KDp2p_UDPSocket *socket)
{
	this->socket = socket;
	destroyThread = false;
	Start();
}

KDp2p_SentMessageQueue::~KDp2p_SentMessageQueue()
{
	destroyThread = true;
	WaitThread();
	queueMutex.Lock();
	while (messages.size()!=0)
	{
		delete messages[0];
		messages.pop_front();
	}
	queueMutex.Unlock();
}

void KDp2p_SentMessageQueue::AddMessage(KDp2p_Message *message)
{
	// On destroy, we don't allow to had messages anymore.
	if (destroyThread)
		return;

	queueMutex.Lock();
	//// Problème: il faudrait vérifier si on peut remplacer un autre message de la queue par celui là, mais celà nécessiterait de connaitre l'adresse IP aussi.
	//// Oki, c bon maintenant que l'address est dedans... à faire!
	messages.push_back(message);
	queueMutex.Unlock();
}

void KDp2p_SentMessageQueue::Run()
{
	while (!destroyThread)
	{
		while (!destroyThread)
		{
			queueMutex.Lock();
			if (messages.size()==0)
			{
				queueMutex.Unlock();
				break;
			}
			KDp2p_Message *messageToSend = messages[0];
			queueMutex.Unlock();

			messageToSend->UDPSend(socket, messageToSend->GetAddress());
			delete messageToSend;
			
			queueMutex.Lock();
			messages.pop_front();
			queueMutex.Unlock();
		}
		SDL_Delay(1);
	}

	// On destroy, we want to send the last messages...
	while (true)
	{
		queueMutex.Lock();
		if (messages.size()==0)
		{
			queueMutex.Unlock();
			break;
		}
		KDp2p_Message *messageToSend = messages[0];
		queueMutex.Unlock();

		messageToSend->UDPSend(socket, messageToSend->GetAddress());
		delete messageToSend;
		
		queueMutex.Lock();
		messages.pop_front();
		queueMutex.Unlock();
	}
}
