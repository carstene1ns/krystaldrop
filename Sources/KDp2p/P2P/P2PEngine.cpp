#include "P2PEngine.h"

#include "AllPeers.h"
#include "../Network/Network.h"
#include "../Network/UDPSocket.h"
#include "../Network/ReceivedMessageQueue.h"
#include "../Network/SentMessageQueue.h"
#include "../Network/Message.h"
#include "ConnectionManager.h"
#include "DialogManager.h"

#include "../../KDpp/Tools/LogFile.h"

int KDp2p_P2PEngine::maxPeerNumberInFile = 5000;
int KDp2p_P2PEngine::engineVersion = 1;
int KDp2p_P2PEngine::peerFileVersion = 1;

KDp2p_P2PEngine::EngineMessageHandler::EngineMessageHandler() : messageHandler(0), isMessageAcquired(0)
{

}

KDp2p_P2PEngine::EngineMessageHandler::EngineMessageHandler(KDp2p_MessageHandler *_messageHandler, bool _isMessageAcquired) : messageHandler(_messageHandler), isMessageAcquired(_isMessageAcquired)
{

}

KDp2p_P2PEngine::EngineMessageHandler::~EngineMessageHandler()
{

}


KDp2p_P2PEngine::KDp2p_P2PEngine() : KDp2p_Thread(), state(stopped), connectionTimeOut(10000), connectionKeepAliveTime(4000), checkTimeOutInterval(1000), nextCheckTimeOutTime(0), peersList(0), network(0), socket(0), recvQueue(0), sendQueue(0), connectionManager(0), autoDestroy(false)
{
	
}

KDp2p_P2PEngine::~KDp2p_P2PEngine()
{
}

int KDp2p_P2PEngine::getMaxPeerNumberInFile()
{
	return maxPeerNumberInFile;
}

int KDp2p_P2PEngine::getEngineVersion()
{
	return engineVersion;
}

int KDp2p_P2PEngine::getPeerFileVersion()
{
	return peerFileVersion;
}

void KDp2p_P2PEngine::Init(int gameId, short int port)
{
	autoDestroy = false;
	state = stopped;

	myId.PickRandomId(gameId);

	peersList = new KDp2p_AllPeers();
	peersList->Load("peerList.p2p");

	network = KDp2p_Network::initNetwork();
	socket = new KDp2p_UDPSocket(port);
	sendQueue = new KDp2p_SentMessageQueue(socket);
	recvQueue = new KDp2p_ReceivedMessageQueue(socket);
	connectionManager = new KDp2p_ConnectionManager(this);
	dialogManager = new KDp2p_DialogManager(this);

	Start();
}

void KDp2p_P2PEngine::Close()
{
	StopThread();

	delete dialogManager;
	delete connectionManager;
	delete sendQueue;
	delete recvQueue;
	delete socket;
	network->closeNetwork();

	peersList->Save("peerList.p2p");
	delete peersList;
}

void KDp2p_P2PEngine::Ping(KDp2p_NetworkAddress *address)
{
	KDp2p_Message *message = new KDp2p_Message();
	message->AddChar('P');
	message->AddChar('I');
	message->AddChar('N');
	message->AddChar('G');
	message->SetAddress(*address);
	sendQueue->AddMessage(message);
}

void KDp2p_P2PEngine::ProcessMessages()
{
	while (true)
	{
		KDp2p_Message *message = recvQueue->PeekMessage();
		if (!message)
			break;

		int messageId = message->GetInt();

		// ICI!!!!!
		// 1 TROUVER LE MESSAGEHANDLER DANS messageHandlers
		// 2 SI IL EXISTE, LANCER LA METHODE HandleMessage

		///// TO REMOVE!!!!!!!
		/**
		char messageType[5];
		messageType[0] = message->GetByte();
		messageType[1] = message->GetByte();
		messageType[2] = message->GetByte();
		messageType[3] = message->GetByte();
		messageType[4] = 0;

		
		if (!strcmp(messageType, "PING"))
		{
			ProcessPing(message);
			delete message;
		}
		else if (!strcmp(messageType, "CORE") && connectionManager != 0)
		{
			connectionManager->RecvCOREMessage(message);
			peersList->AddPeer(message->GetAddress());
			delete message;
		}
		else if (!strcmp(messageType, "COAC") && connectionManager != 0)
		{
			connectionManager->RecvCOACMessage(message);
			delete message;
		}
		else if (!strcmp(messageType, "HELO") && connectionManager != 0)
		{
			connectionManager->RecvHELOMessage(message);
			delete message;
		}
		else if (!strcmp(messageType, "STCO") && connectionManager != 0)
		{
			connectionManager->RecvSTCOMessage(message);
			delete message;
		}
		else if (!strcmp(messageType, "QUES") && dialogManager != 0)
		{
			dialogManager->ProcessQuestion(message);
			// Note: the message is not destroyed... it is owned by the dialog... and is already destroyed!
		}
		else if (!strcmp(messageType, "ANSW") && dialogManager != 0)
		{
			dialogManager->ProcessAnswer(message);
			// Note: the message is not destroyed... it is owned by the dialog.
		}
		*/
		
		
	}

	unsigned int time = SDL_GetTicks();

	if (nextCheckTimeOutTime < time)
	{
		connectionManager->ComputeTimeOut();
		nextCheckTimeOutTime = time + checkTimeOutInterval;

		dialogManager->ProcessTimeOut();
	}
}

void KDp2p_P2PEngine::ProcessPing(KDp2p_Message *message)
{
	KDp2p_Message *answer = new KDp2p_Message();
	answer->AddChar('P');
	answer->AddChar('O');
	answer->AddChar('N');
	answer->AddChar('G');
	answer->SetAddress(*(message->GetAddress()));
	sendQueue->AddMessage(answer);

	peersList->AddPeer(message->GetAddress());

	printf("PING from %s\n",message->GetAddress()->ToString().c_str());
}

void KDp2p_P2PEngine::ProcessPong(KDp2p_Message *message)
{
	peersList->AddPeer(message->GetAddress());
}

KDp2p_SentMessageQueue *KDp2p_P2PEngine::GetSendQueue()
{
	return sendQueue;
}

KDp2p_ReceivedMessageQueue *KDp2p_P2PEngine::GetRecvQueue()
{
	return recvQueue;
}

KDp2p_DialogManager *KDp2p_P2PEngine::GetDialogManager()
{
	return dialogManager;
}

int KDp2p_P2PEngine::GetConnectionTimeOut()
{
	return connectionTimeOut;
}

int KDp2p_P2PEngine::GetConnectionKeepAliveTime()
{
	return connectionKeepAliveTime;
}

void KDp2p_P2PEngine::Connect(KDp2p_NetworkAddress *address, KDp2p_ConnectionListener *listener)
{
	connectionManager->AddConnection(address, listener);
}

void KDp2p_P2PEngine::Run()
{
	// First, we should find peers from the peers list
	peersList->ResetPointer();

	KDp2p_NetworkAddress firstPeer;
	bool failed = false;

	state = lookingForPeer;

	// Note this will fail if there are no peers... there should sure be at least one.
	do
	{
		KDp2p_NetworkAddress peer = peersList->GetPeer();
		Ping(&peer);

		KDp2p_Message *message = recvQueue->PeekMessage();
		if (message)
		{
			firstPeer = *(message->GetAddress());
			KD_LogFile::printf2("First peer who answered: %s\n",firstPeer.ToString().c_str());
			break;
		}

		failed = !(peersList->Next());
	} while (!failed && !autoDestroy);

	// Destroys if asked
	if (autoDestroy)
	{
		autoDestroyDone = true;
		return;
	}

	// If we couldn't find any peer... That's bad, we'll have to wait for a peer to contact us!
	if (failed)
	{
		state = failedToFindPeer;
		KD_LogFile::printf2("Failed to fin a peer, in waiting mode.\n");
	}
	else
	{
		state = connected;
		// Message HELLO I'm NEW (HNEW).
		

	}

	while (!autoDestroy)
	{
		ProcessMessages();
		SDL_Delay(1);

		// ICI!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	autoDestroyDone = true;
}

void KDp2p_P2PEngine::StopThread()
{
	autoDestroy = true;

	while (!autoDestroyDone)
	{
		SDL_Delay(1);
	}

	autoDestroyDone = false;
}

void KDp2p_P2PEngine::AddMessageHandler(int messageId, KDp2p_MessageHandler *messageHandler, bool isAcquiringMessage)
{
	messageHandlers[messageId] = EngineMessageHandler(messageHandler, isAcquiringMessage);
}

void KDp2p_P2PEngine::AddMessageHandler(char messageChar[5], KDp2p_MessageHandler *messageHandler, bool isAcquiringMessage)
{
	unsigned int id = (messageChar[0]<<24) + (messageChar[1]<<16) + (messageChar[2]<<8) + messageChar[3];
	AddMessageHandler(id, messageHandler, isAcquiringMessage);
}

void KDp2p_P2PEngine::RemoveMessageHandler(int messageId)
{
	messageHandlers.erase(messageId);
}

void KDp2p_P2PEngine::RemoveMessageHandler(char messageChar[5])
{
	unsigned int id = (messageChar[0]<<24) + (messageChar[1]<<16) + (messageChar[2]<<8) + messageChar[3];
	RemoveMessageHandler(id);
}
