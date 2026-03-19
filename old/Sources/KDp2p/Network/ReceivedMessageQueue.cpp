#include "ReceivedMessageQueue.h"

#include "Packet.h"
#include "Message.h"
#include "Network.h"

#include <SDL/SDL.h>
#include "../Tools/NetException.h"
#include "../../KDpp/Tools/Logfile.h"

KDp2p_ReceivedMessageQueue::KDp2p_ReceivedMessageQueue(KDp2p_UDPSocket *socket)
{
	this->socket = socket;
	destroyThread = false;
	Start();
}

KDp2p_ReceivedMessageQueue::~KDp2p_ReceivedMessageQueue()
{
	destroyThread = true;
	WaitThread();
	while (messagesReceiving.size()!=0)
	{
		delete messagesReceiving[0];
		messagesReceiving.pop_front();
	}

	queueMutex.Lock();
	while (messagesReceived.size()!=0)
	{
		delete messagesReceived[0];
		messagesReceived.pop_front();
	}
	queueMutex.Unlock();
}

void KDp2p_ReceivedMessageQueue::Run()
{
	KDp2p_Packet *packet = 0;
	while (!destroyThread)
	{
		while (!destroyThread)
		{
			if (!packet)
				packet = new KDp2p_Packet();

			bool ret;
			try
			{
				ret = packet->UDPReceive(socket);
			}
			catch (KDp2p_NetException e)
			{
				KD_LogFile::printf2("%s", e.ToString().c_str());
				ret = false;
			}
			if (ret == false)
				break;

			InsertPacket(packet);
			packet = 0;
			DiscardOldMessages();
		}
		SDL_Delay(1);
	}

	if (packet)
		delete packet;
}

void KDp2p_ReceivedMessageQueue::InsertPacket(KDp2p_Packet *packet)
{
	if (packet->GetTotalPacketNumber() == 1)
	{
		queueMutex.Lock();
		messagesReceived.push_back(new KDp2p_Message(packet));
		messagesReceived[messagesReceived.size()-1]->UpdateTime();
		queueMutex.Unlock();
		return;
	}

	int i;
	bool found = false;

	for (i=0; i<messagesReceiving.size(); i++)
	{
		if (messagesReceiving[i]->GetMsgId() == packet->GetMessageID() && *(messagesReceiving[i]->GetAddress()) == *(packet->GetAddress()))
		{
			bool ret = messagesReceiving[i]->AddReceivedPacket(packet);
			messagesReceiving[i]->UpdateTime();
			if (ret == true)
			{
				queueMutex.Lock();
				messagesReceived.push_back(messagesReceiving[i]);
				queueMutex.Unlock();
				messagesReceiving.erase(messagesReceiving.begin()+i);
			}
			found = true;
			break;
		}
	}
	
	if (found == false)
	{
		messagesReceiving.push_back(new KDp2p_Message(packet));
	}
}

void KDp2p_ReceivedMessageQueue::DiscardOldMessages()
{
	int timeOut = KDp2p_Network::getNetwork()->getReceiveTimeOut();
	int time = SDL_GetTicks();
	
	int i=0;
	while (i != messagesReceiving.size())
	{
		if (messagesReceiving[i]->GetTime() + timeOut < SDL_GetTicks())
		{
			messagesReceiving.erase(messagesReceiving.begin()+i);
			KD_LogFile::printf2("Warning, UDP message from %s discarded because a packet has not being received (packet timeout)", messagesReceiving[i]->GetAddress()->ToString().c_str());
		}
		else
			i++;
	}
}

KDp2p_Message *KDp2p_ReceivedMessageQueue::PeekMessage()
{
	if (messagesReceived.size() == 0)
		return NULL;
	else
	{
		KDp2p_Message *message = messagesReceived[0];
		messagesReceived.pop_front();
		return message;
	}
}
