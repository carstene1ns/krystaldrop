#include "ConnectionManager.h"

#include "../Network/Message.h"
#include "P2PEngine.h"
#include "../Network/SentMessageQueue.h"
#include "ConnectionListener.h"
#include "AllPeers.h"

#include "../../KDpp/Tools/Logfile.h"

/*bool KDp2p_ConnectionManager::TimedAddress::operator < (const TimedAddress &that) const
{
	if (address < that.address)
		return true;
	else
		return false;
}*/

bool KDp2p_ConnectionManager::TimedAddress::RemoveConnectionHandler(KDp2p_ConnectionHandler *handler)
{
	deque<KDp2p_ConnectionHandler*>::iterator itHandler = handlers.begin();

	while (itHandler != handlers.end())
	{
		if ((*itHandler) == handler)
		{
			// Found the connexion handler for the given address!
			// Remove the ConnectionHandler
			itHandler = handlers.erase(itHandler);
			return true;
		}
		itHandler++;
	}
	return false;
}

KDp2p_ConnectionManager::KDp2p_ConnectionManager(KDp2p_P2PEngine *engine)
{
	this->engine = engine;
	timeLastSent = 0;
	posLastSent = 0;
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
	KD_LogFile::printf2("Sending STCO to node %s\n",address->ToString().c_str());

	engine->GetSendQueue()->AddMessage(message);
}

deque<KDp2p_NetworkAddress>::iterator KDp2p_ConnectionManager::FindConnectionInAddressToSend(const KDp2p_NetworkAddress &address)
{
	deque<KDp2p_NetworkAddress>::iterator it = addressToSend.begin();

	while (it != addressToSend.end())
	{
		if ((*it) == address)
			break;
		it++;
	}
	return it;
}

deque<KDp2p_ConnectionManager::TimedAddress>::iterator KDp2p_ConnectionManager::FindConnectionInAddressTryingToConnectTo(const KDp2p_NetworkAddress &address)
{
	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	while (it != addressTryingToConnectTo.end())
	{
		if (it->address == address)
			break;
		it++;
	}
	return it;
}

deque<KDp2p_ConnectionManager::TimedAddress>::iterator KDp2p_ConnectionManager::FindConnectionInLastReceivedFromAddress(const KDp2p_NetworkAddress &address)
{
	deque<TimedAddress>::iterator it = lastReceivedFromAddress.begin();

	while (it != lastReceivedFromAddress.end())
	{
		if (it->address == address)
			break;
		it++;
	}
	return it;
}

KDp2p_ConnectionHandler *KDp2p_ConnectionManager::AddConnection(const KDp2p_NetworkAddress &address, KDp2p_ConnectionListener *listener)
{
	KDp2p_ConnectionHandler *connectionHandler = new KDp2p_ConnectionHandler();
	connectionHandler->address = address;
	connectionHandler->listener = listener;

	// First, is the address already part of the connections handled?
	deque<TimedAddress>::iterator it = FindConnectionInLastReceivedFromAddress(address);

	if (it != lastReceivedFromAddress.end())
	{
		(*it).handlers.push_back(connectionHandler);
		return connectionHandler;
	}
	else
	{
		it = FindConnectionInAddressTryingToConnectTo(address);
		if (it != addressTryingToConnectTo.end())
		{
			(*it).handlers.push_back(connectionHandler);
			return connectionHandler;
		}	
	}

	// Let's add the message to the addressTryingToConnectTo deque
	TimedAddress destination;

	destination.address = address;
	destination.handlers.push_back(connectionHandler);
	destination.time = SDL_GetTicks() + engine->GetConnectionTimeOut();
	addressTryingToConnectTo.push_back(destination);

	// Let's send the request message
	KDp2p_Message *message = new KDp2p_Message();
	message->AddChar('C');
	message->AddChar('O');
	message->AddChar('R');
	message->AddChar('E');
	message->SetAddress(address);
	engine->GetSendQueue()->AddMessage(message);
	KD_LogFile::printf2("Requesting connection for node %s\n",address.ToString().c_str());
	return connectionHandler;
}

void KDp2p_ConnectionManager::SendUpdateMessages()
{
	int time = (int) SDL_GetTicks();

	if (addressToSend.size() == 0)
		return;

	int newPos = (time%(engine->GetConnectionKeepAliveTime()))*((int)addressToSend.size())/engine->GetConnectionKeepAliveTime();

	if (newPos == posLastSent && time - timeLastSent < engine->GetConnectionKeepAliveTime())
		return;
	
	if (time - timeLastSent >= engine->GetConnectionKeepAliveTime())
	{
		// If we turned around the timer, let's send the HELO messages only once!
		newPos = posLastSent;
	}

	do
	{
		posLastSent = (posLastSent+1)%((int)addressToSend.size());
		
		// Send the message
		KDp2p_Message *message = new KDp2p_Message();
		message->AddChar('H');
		message->AddChar('E');
		message->AddChar('L');
		message->AddChar('O');
		message->SetAddress(addressToSend[posLastSent]);
		KD_LogFile::printf2("Sending update message to node %s\n",addressToSend[posLastSent].ToString().c_str());
		engine->GetSendQueue()->AddMessage(message);

	} while (posLastSent != newPos);

	timeLastSent = time;
}

void KDp2p_ConnectionManager::CloseConnection(KDp2p_ConnectionHandler *handler)
{
	// Must close the connection only if there are no more ConnectionHandlers on it!
	deque<KDp2p_NetworkAddress>::iterator it = FindConnectionInAddressToSend(handler->address);
	printf("Closing Connection to %s\n",handler->address.ToString().c_str());
	
	if (it != addressToSend.end())
	{
		// Found in addressToSend!
		
		// Let's remove the given handler from lastReceivedFromAddress
		deque<TimedAddress>::iterator it2 = FindConnectionInLastReceivedFromAddress(handler->address);
		
		(*it2).RemoveConnectionHandler(handler);

		// Are there still ConnectionHandlers for this connection?
		if ((*it2).handlers.empty())
		{
			// Ok, no more connectionHandlers, let's destroy it!
			// mmm... first let's see if we must decrease posLastSent
			for (int i=0; i<posLastSent; i++)
			{
				if (addressToSend[i] == handler->address)
				{
					posLastSent = (posLastSent-1)%engine->GetConnectionKeepAliveTime();
					break;
				}
			}
			
			SendSTCOMessage(&(handler->address));
			it = addressToSend.erase(it);
			it2 = lastReceivedFromAddress.erase(it2);
		}
	}
	else
	{
		deque<TimedAddress>::iterator it3 = FindConnectionInAddressTryingToConnectTo(handler->address);
		if (it3 != addressTryingToConnectTo.end())
		{
			// Let's remove the given handler
			(*it3).RemoveConnectionHandler(handler);

			// Are there still ConnectionHandlers for this connection?
			if ((*it3).handlers.empty())
			{
				it3 = addressTryingToConnectTo.erase(it3);
			}
		}	
	}

	// Deletes the handler!
	delete handler;
}

void KDp2p_ConnectionManager::CloseAllConnections()
{
	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	while (it != addressTryingToConnectTo.end())
	{
		SendSTCOMessage(&(it->address));
		it = addressTryingToConnectTo.erase(it);
	}

	deque<KDp2p_NetworkAddress>::iterator it2 = addressToSend.begin();

	while (it2 != addressToSend.end())
	{
		SendSTCOMessage(&(*it2));
		it2 = addressToSend.erase(it2);
	}

	lastReceivedFromAddress.clear();
}

void KDp2p_ConnectionManager::RecvCOREMessage(KDp2p_Message *message)
{
	
	unsigned int time = SDL_GetTicks();

	// Adds the requester to the AllPeers list
	engine->GetPeersList()->AddPeer(message->GetAddress());

	// Adds the requester to the connection list
	TimedAddress requester;

	requester.address = *(message->GetAddress());
	//requester.listener = 0;
	requester.time = SDL_GetTicks() + engine->GetConnectionTimeOut();
	lastReceivedFromAddress.push_back(requester);

	addressToSend.push_back(*(message->GetAddress()));

	// Sends a COAC message to the requester
	KDp2p_Message *answer = new KDp2p_Message();
	answer->AddChar('C');
	answer->AddChar('O');
	answer->AddChar('A');
	answer->AddChar('C');
	answer->SetAddress(requester.address);
	KD_LogFile::printf2("Received Connection Request from node %s\nSending Connection Accepted message.\n",requester.address.ToString().c_str());
	engine->GetSendQueue()->AddMessage(answer);
}

void KDp2p_ConnectionManager::RecvCOACMessage(KDp2p_Message *message)
{
	unsigned int time = SDL_GetTicks();


	deque<TimedAddress>::iterator it = addressTryingToConnectTo.begin();

	bool found = false;

	while (it != addressTryingToConnectTo.end())
	{
		KD_LogFile::printf2("%s\n", it->address.ToString().c_str());
		if (it->address == *(message->GetAddress()))
		{
			found = true;
			//if (it->listener != 0)
			//	it->listener->ConnectionSucceeded(it->address);
			KD_LogFile::printf2("Received Connection Acknowledgement from peer %s\n",message->GetAddress()->ToString().c_str());

			deque<KDp2p_ConnectionHandler*>::iterator itHandlers = it->handlers.begin();
			while (itHandlers != it->handlers.end())
			{
				//(*itHandlers)->listener->ConnectionSucceeded((*itHandlers)->address);
				(*itHandlers)->listener->ConnectionSucceeded(*itHandlers);
				itHandlers++;
			}

			addressToSend.push_back(it->address);
			//addressToSend[addressToSend.size()-1].time = time + engine->GetConnectionKeepAliveTime();

			lastReceivedFromAddress.push_back(*it);
			lastReceivedFromAddress[lastReceivedFromAddress.size()-1].time = time + engine->GetConnectionTimeOut();

			// Adds the peer to the AllPeers list
			engine->GetPeersList()->AddPeer(message->GetAddress());

			it = addressTryingToConnectTo.erase(it);

			break;
		}
		it++;
	}

	if (!found)
		KD_LogFile::printf2("Warning! Received a COAC message from node %s. This is unexpected. The connection might have timed out.\n",message->GetAddress()->ToString().c_str());
}

void KDp2p_ConnectionManager::RecvHELOMessage(KDp2p_Message *message)
{
	for (unsigned int i=0; i<lastReceivedFromAddress.size(); i++)
	{
		if (lastReceivedFromAddress[i].address == *(message->GetAddress()))
		{
			lastReceivedFromAddress[i].time = SDL_GetTicks() + engine->GetConnectionTimeOut();
			break;
		}
	}
}

void KDp2p_ConnectionManager::RecvSTCOMessage(KDp2p_Message *message)
{
	deque<TimedAddress>::iterator it = FindConnectionInLastReceivedFromAddress(*(message->GetAddress()));

	printf("Received STCO message from %s\n", message->GetAddress()->ToString().c_str());

	if (it != lastReceivedFromAddress.end())
	{
		//if (it->address == *(message->GetAddress()))
		//{
		deque<KDp2p_ConnectionHandler*>::iterator itHandlers = it->handlers.begin();
		while (itHandlers != it->handlers.end())
		{
			//(*itHandlers)->listener->ConnectionClosedByPeer(*(message->GetAddress()));
			(*itHandlers)->listener->ConnectionClosedByPeer(*itHandlers);
			itHandlers++;
		}

		it = lastReceivedFromAddress.erase(it);
		//}

		deque<KDp2p_NetworkAddress>::iterator it2 = FindConnectionInAddressToSend(*(message->GetAddress()));

#ifdef DEBUG
		if (it2 == addressToSend.end())
		{
			KD_LogFile::printf2("WARNING! In KDp2p_ConnectionManager::RecvSTCOMessage, the connection has been removed from lastReceivedFromAddress but hasn't been found in addressToSend!\nThis should never happen!\n");
		}
#endif
		it2 = addressToSend.erase(it2);
	}
	else
	{
		KD_LogFile::printf2("Warning, could not close the connection to %s, connection not found!", message->GetAddress()->ToString().c_str());
		return;
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
			KD_LogFile::printf2("Connection Request to node %s timed out.\n",it->address.ToString().c_str());
			//if (it->listener != 0)
			//	it->listener->ConnectionFailed(it->address);
			deque<KDp2p_ConnectionHandler*>::iterator itHandlers = it->handlers.begin();
			while (itHandlers != it->handlers.end())
			{
				//(*itHandlers)->listener->ConnectionFailed(it->address);
				(*itHandlers)->listener->ConnectionFailed(*itHandlers);
				// let's delete this handler
				delete (*itHandlers);
				itHandlers++;
			}

			it = addressTryingToConnectTo.erase(it);
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
			//if (it->listener != 0)
			//	it->listener->ConnectionInterrupted(it->address);

			KD_LogFile::printf2("Connection to node %s timed out.\n",it->address.ToString().c_str());

			deque<KDp2p_ConnectionHandler*>::iterator itHandlers = it->handlers.begin();
			while (itHandlers != it->handlers.end())
			{
				//(*itHandlers)->listener->ConnectionInterrupted(it->address);
				(*itHandlers)->listener->ConnectionInterrupted(*itHandlers);
				// let's delete this handler
				delete (*itHandlers);
				itHandlers++;
			}
			
			// now we must erase the address in the addressToSend deque
			deque<KDp2p_NetworkAddress>::iterator it2 = addressToSend.begin();
			while (it2 != addressToSend.end())
			{
				//KD_LogFile::printf2("comparing it:%s to it2:%s\n",it->address.ToString().c_str(),it2->address.ToString().c_str());	
				if ((*it2) == it->address)
				{
					KD_LogFile::printf2("Removed %s from SendQueue.\n",it2->ToString().c_str());
					it2 = addressToSend.erase(it2);
					break;
				}
				it2++;
			}

			it = lastReceivedFromAddress.erase(it);
		}
		else
			it++;
	}

}

void KDp2p_ConnectionManager::HandleMessage(KDp2p_Message *message, int id)
{
	switch (id)
	{
	case CORE_MESSAGEID:
		RecvCOREMessage(message);
		break;
	case COAC_MESSAGEID:
		RecvCOACMessage(message);
		break;
	case HELO_MESSAGEID:
		RecvHELOMessage(message);
		break;
	case STCO_MESSAGEID:
		RecvSTCOMessage(message);
		break;
	}
}
