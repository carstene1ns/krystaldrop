#include "ConnectionManager.h"

#include "../Network/Message.h"
#include "P2PEngine.h"
#include "../Network/SentMessageQueue.h"
#include "ConnectionListener.h"

#include "../../KDpp/Tools/Logfile.h"

KDp2p_ConnectionManager::KDp2p_ConnectionManager(KDp2p_P2PEngine *engine)
{
	this->engine = engine;
}

KDp2p_ConnectionManager::~KDp2p_ConnectionManager()
{
	CloseAllConnections();
}

void KDp2p_ConnectionManager::SendSTCOMessage(KDp2p_NetworkAddress *address)
{
	KDp2p_Message *message = new KDp2p_Message();
	message->AddChar('S');
	message->AddChar('T');
	message->AddChar('C');
	message->AddChar('O');
	message->SetAddress(*address);
	engine->GetSendQueue()->AddMessage(message);
}

void KDp2p_ConnectionManager::AddConnection(KDp2p_NetworkAddress *address, KDp2p_ConnectionListener *listener)
{
	// Let's add the message to the addressTryingToConnectTo deque
	TimedAddress destination;

	destination.address = *address;
	destination.listener = listener;
	destination.time = SDL_GetTicks() + engine->GetConnectionTimeOut();
	addressTryingToConnectTo.push_back(destination);

	// Let's send the request message
	KDp2p_Message *message = new KDp2p_Message();
	message->AddChar('C');
	message->AddChar('O');
	message->AddChar('R');
	message->AddChar('E');
	message->SetAddress(*address);
	engine->GetSendQueue()->AddMessage(message);
}

void KDp2p_ConnectionManager::SendUpdateMessages()
{
	unsigned int time = SDL_GetTicks();

	while (addressToSend[0].time < time)
	{
		// Send the message
		KDp2p_Message *message = new KDp2p_Message();
		message->AddChar('H');
		message->AddChar('E');
		message->AddChar('L');
		message->AddChar('O');
		message->SetAddress(addressToSend[0].address);
		engine->GetSendQueue()->AddMessage(message);

		// Put the first message in last position in queue
		addressToSend[0].time = time + engine->GetConnectionKeepAliveTime();
		addressToSend.push_back(addressToSend[0]);
		addressToSend.pop_front();
	}
}

void KDp2p_ConnectionManager::CloseConnection(KDp2p_NetworkAddress *address)
{
	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	while (it != addressTryingToConnectTo.end())
	{
		if (it->address == *address)
		{
			SendSTCOMessage(address);
			addressTryingToConnectTo.erase(it);
			return;
		}
		it++;
	}

	it = addressToSend.begin();

	while (it != addressToSend.end())
	{
		if (it->address == *address)
		{
			SendSTCOMessage(address);
			addressToSend.erase(it);
			break;
		}
		it++;
	}

	if (it == addressToSend.end())
	{
		KD_LogFile::printf2("Warning, could not close the connection to %s, connection not found!", address->ToString().c_str());
		return;
	}

	it = lastReceivedFromAddress.begin();

	while (it != lastReceivedFromAddress.end())
	{
		if (it->address == *address)
		{
			SendSTCOMessage(address);
			lastReceivedFromAddress.erase(it);
			break;
		}
		it++;
	}
}

void KDp2p_ConnectionManager::CloseAllConnections()
{
	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	while (it != addressTryingToConnectTo.end())
	{
		SendSTCOMessage(&it->address);
		addressTryingToConnectTo.erase(it);
	}

	it = addressToSend.begin();

	while (it != addressToSend.end())
	{
		SendSTCOMessage(&it->address);
		addressToSend.erase(it);
	}

	lastReceivedFromAddress.clear();
}

void KDp2p_ConnectionManager::RecvCOREMessage(KDp2p_Message *message)
{
	
	unsigned int time = SDL_GetTicks();

	// Adds the requester to the connection list
	TimedAddress requester;

	requester.address = *(message->GetAddress());
	requester.listener = 0;
	requester.time = SDL_GetTicks() + engine->GetConnectionTimeOut();
	lastReceivedFromAddress.push_back(requester);

	requester.time = SDL_GetTicks() + engine->GetConnectionKeepAliveTime();
	addressToSend.push_back(requester);

	// Sends a COAC message to the requester
	KDp2p_Message *answer = new KDp2p_Message();
	answer->AddChar('C');
	answer->AddChar('O');
	answer->AddChar('A');
	answer->AddChar('C');
	answer->SetAddress(requester.address);
	engine->GetSendQueue()->AddMessage(answer);
}

void KDp2p_ConnectionManager::RecvCOACMessage(KDp2p_Message *message)
{
	unsigned int time = SDL_GetTicks();

	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	while (it != addressTryingToConnectTo.end())
	{
		if (it->address == *(message->GetAddress()))
		{
			if (it->listener != 0)
				it->listener->ConnectionSucceeded(it->address);
			
			addressToSend.push_back(*it);
			addressToSend[addressToSend.size()-1].time = time + engine->GetConnectionKeepAliveTime();

			addressTryingToConnectTo.push_back(*it);
			addressTryingToConnectTo[addressTryingToConnectTo.size()-1].time = time + engine->GetConnectionTimeOut();

			addressTryingToConnectTo.erase(it);
			
			break;
		}
		it++;
	}

	// DEBUG
	if (it == addressTryingToConnectTo.end())
		KD_LogFile::printf2("Warning, received a COAC from an unknown host, the COAC message must have timed out.");
}

void KDp2p_ConnectionManager::RecvHELOMessage(KDp2p_Message *message)
{
	for (unsigned int i=0; i<lastReceivedFromAddress.size(); i++)
	{
		if (addressToSend[i].address == *(message->GetAddress()))
		{
			addressToSend[i].time = SDL_GetTicks() + engine->GetConnectionTimeOut();
			break;
		}
	}
}

void KDp2p_ConnectionManager::RecvSTCOMessage(KDp2p_Message *message)
{
	deque<TimedAddress>::iterator it = addressToSend.begin();

	while (it != addressToSend.end())
	{
		if (it->address == *(message->GetAddress()))
		{
			if (it->listener != 0)
				it->listener->ConnectionClosedByPeer(*(message->GetAddress()));
			addressToSend.erase(it);
			break;
		}
		it++;
	}

	if (it == addressToSend.end())
	{
		KD_LogFile::printf2("Warning, could not close the connection to %s, connection not found!", message->GetAddress()->ToString().c_str());
		return;
	}

	it = lastReceivedFromAddress.begin();

	while (it != lastReceivedFromAddress.end())
	{
		if (it->address == *(message->GetAddress()))
		{
			lastReceivedFromAddress.erase(it);
			break;
		}
		it++;
	}
}

void KDp2p_ConnectionManager::ComputeTimeOut()
{
	// Check COAC time out (time out for receiving a "connection accepted" message).
	unsigned int time = SDL_GetTicks();
	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	while (it != addressTryingToConnectTo.end())
	{
		if (time > it->time)
		{
			if (it->listener != 0)
				it->listener->ConnectionFailed(it->address);
			addressTryingToConnectTo.erase(it);
		}
		else
			it++;
	}

	// Check HELO time out (time out for receiving a keepalive connection message)
	it = lastReceivedFromAddress.begin();

	while (it != lastReceivedFromAddress.end())
	{
		if (time > it->time)
		{
			if (it->listener != 0)
				it->listener->ConnectionInterrupted(it->address);
			
			// now we must erase the address in the addressToSend deque
			deque<TimedAddress>::iterator it2 = addressToSend.begin();
			while (it2 != addressToSend.end())
			{
				if (it2->address == it->address)
				{
					addressToSend.erase(it2);
					break;
				}
				it2++;
			}

			lastReceivedFromAddress.erase(it);
		}
		else
			it++;
	}

}

void KDp2p_ConnectionManager::HandleMessage(KDp2p_MessageHandler *message, int id)
{

}