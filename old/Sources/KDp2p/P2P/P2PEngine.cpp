#include "P2PEngine.h"

#include "AllPeers.h"
#include "../Network/Network.h"
#include "../Network/UDPSocket.h"
#include "../Network/ReceivedMessageQueue.h"
#include "../Network/SentMessageQueue.h"
#include "../Network/Message.h"
#include "ConnectionManager.h"
#include "DialogManager.h"
#include "MessageHandler.h"
#include "PingDialogFactory.h"
#include "PingDialog.h"
#include "PingCounter.h"
#include "BTree/BTree.h"
#include "BTree/SubscribeToNodeDialogFactory.h"
#include "BTree/SubscribeToNodeDialog.h"
#include "BTree/SerializableString.h"

#include "../../KDpp/Tools/Logfile.h"

int KDp2p_P2PEngine::maxPeerNumberInFile = 5000;
int KDp2p_P2PEngine::engineVersion = 1;
int KDp2p_P2PEngine::peerFileVersion = 1;

#define PING_MESSAGEID ((((('P'<<24) + ('I'<<16) + ('N'<<8) + 'G'))))

KDp2p_P2PEngine::EngineMessageHandler::EngineMessageHandler()
    : isMessageAcquired(0), messageHandler(0)
{
	
}

KDp2p_P2PEngine::EngineMessageHandler::EngineMessageHandler(KDp2p_MessageHandler *_messageHandler, bool _isMessageAcquired)
    : isMessageAcquired(_isMessageAcquired), messageHandler(_messageHandler)
{

}

KDp2p_P2PEngine::EngineMessageHandler::~EngineMessageHandler()
{

}

KDp2p_P2PEngine::KDp2p_P2PEngine() :
    KDp2p_Thread(),
    peersList(0),
    network(0),
    socket(0),
    sendQueue(0),
    recvQueue(0),
    connectionManager(0),
    state(stopped),
    connectionTimeOut(10000),
    connectionKeepAliveTime(4000),
    checkTimeOutInterval(1000),
    nextCheckTimeOutTime(0),
    autoDestroy(false)
{
	peerListFileName = "peerList.p2p";
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
//<<<<<<< P2PEngine.cpp
	// init the random generator (used to generate the random Id...)
	srand( (unsigned)time( NULL ) );

	autoDestroy = false;
	state = stopped;
//=======
//    autoDestroy = false;
//    state = stopped;
//>>>>>>> 1.7

    myId.PickRandomId(gameId);

	peersList = new KDp2p_AllPeers();
	peersList->Load(peerListFileName);

    network = KDp2p_Network::initNetwork();
    socket = new KDp2p_UDPSocket(port);
    sendQueue = new KDp2p_SentMessageQueue(socket);
    recvQueue = new KDp2p_ReceivedMessageQueue(socket);
    connectionManager = new KDp2p_ConnectionManager(this);
    dialogManager = new KDp2p_DialogManager(this);

	// instanciates the BTree used to find peers. Gives him ID number 0
	SetUniqueObject(0, new KDp2p_BTree(this, 128));

	AddMessageHandler(PING_MESSAGEID, this, false);
	// QUES ans ANSW message are handled by the DialogManager which keeps ownership on there lives
	AddMessageHandler(QUES_MESSAGEID, dialogManager, true);
	AddMessageHandler(ANSW_MESSAGEID, dialogManager, true);


    dialogManager->AddFactory(PING_MESSAGEID, new KDp2p_PingDialogFactory());

    Start();
}

void KDp2p_P2PEngine::Close()
{
    StopThread();

    dialogManager->RemoveAndDeleteFactory(PING_MESSAGEID);

	RemoveMessageHandler(ANSW_MESSAGEID);
	RemoveMessageHandler(QUES_MESSAGEID);
	RemoveMessageHandler(PING_MESSAGEID);

	delete (KDp2p_BTree*)(uniqueObjects[0]);

	delete dialogManager;
	delete connectionManager;
	delete sendQueue;
	delete recvQueue;
	delete socket;
	network->closeNetwork();

	peersList->Save(peerListFileName);
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

        map<int, EngineMessageHandler>::iterator it;
        it = messageHandlers.find(messageId);

        if (it != messageHandlers.end())
        {
            it->second.messageHandler->HandleMessage(message, messageId);
            // If the message is not owned by the MessageHandler, deletes it.
            if (it->second.isMessageAcquired == false)
                delete message;
        }


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

KDp2p_AllPeers *KDp2p_P2PEngine::GetPeersList()
{
    return peersList;
}

KDp2p_ConnectionManager *KDp2p_P2PEngine::GetConnectionManager()
{
    return connectionManager;
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
//<<<<<<< P2PEngine.cpp
	connectionManager->AddConnection((*address), listener);
//=======
//    connectionManager->AddConnection(address, listener);
//>>>>>>> 1.7
}

void KDp2p_P2PEngine::Run()
{
    // First, we should find peers from the peers list
    peersList->ResetPointer();

    KDp2p_NetworkAddress firstPeer;
    bool failed = false;

    state = lookingForPeer;


	KDp2p_PingCounter *pingCounter = new KDp2p_PingCounter();

	// Note this will fail if there are no peers... there should sure be at least one.
	// First, let's send all the pings.
	while (!autoDestroy)
	{
		KDp2p_NetworkAddress peer = peersList->GetPeer();
		/// HERE LET'S PUT PINGDIALOG
		KDp2p_PingDialog *pingDialog = new KDp2p_PingDialog(this, pingCounter);
		pingDialog->GetQuestion()->SetAddress(peer);
		pingDialog->SendQuestion();

		ProcessMessages();

		// If an answer is received, stop sending Pings
		if (pingCounter->GetNbPingAnswers() != 0)
			break;
		
		bool isRemaining = peersList->Next();
		if (isRemaining == false)
			break;	
	}

	// Then, let's test for the answers and wait a ping (we wait for all pings to timeout).
	// For V2: wait a few second to receive more pings
	while (!failed && !autoDestroy)
	{
		ProcessMessages();

		// If an answer is received, break.
		if (pingCounter->GetNbPingAnswers() != 0)
			break;
		// If we are no more waiting for answers from the pings send, we failed.
		if (pingCounter->GetPingCounter() == 0)
			failed = true;

		SDL_Delay(1);
	}

	// Removes all the ping answers waited.
	dialogManager->RemoveQuestionsByType(PING_MESSAGEID);

	// Destroys if asked
	if (autoDestroy)
	{
		autoDestroyDone = true;
		return;
	}

	if (!failed)
		firstPeer = pingCounter->GetAddress();

	// Deletes the pingCounter object.
	delete pingCounter;


	// Ok, we've got an address, let's rock!
	// Let's register the connection managing messages.
	AddMessageHandler(COAC_MESSAGEID, connectionManager, false);
	AddMessageHandler(CORE_MESSAGEID, connectionManager, false);
	AddMessageHandler(HELO_MESSAGEID, connectionManager, false);
	AddMessageHandler(STCO_MESSAGEID, connectionManager, false);
	// Let's register the dialogs.
	dialogManager->AddFactory("STNO", new KDp2p_SubscribeToNodeDialogFactory());

	// If we couldn't find any peer... That's bad, we'll have to wait for a peer to contact us!
	if (failed)
	{
		state = failedToFindPeer;
		KD_LogFile::printf2("Failed to find a peer, in waiting mode.\n");

 		((KDp2p_BTree*)uniqueObjects[0])->AddLeafDirectly(0, KDp2p_BPosition(myId), new KDp2p_SerializableString("Coucou ceci est un test!"));
	}
	else
	{
		state = connected;
		KD_LogFile::printf2("Found a peer: %s.\n", firstPeer.ToString().c_str());

		// Ok, let's register our P2PId!
		KDp2p_SubscribeToNodeDialog *newDialog = new KDp2p_SubscribeToNodeDialog(this);
		newDialog->SetNodeToRetrieve(0, KDp2p_BPosition(), KDp2p_BPosition(myId), new KDp2p_SerializableString("Coucou ceci est un test!"));
		newDialog->GetQuestion()->SetAddress(firstPeer);
		newDialog->SendQuestion();	
	}

	while (!autoDestroy)
	{
		ProcessMessages();
		SDL_Delay(1);

		connectionManager->SendUpdateMessages();
		connectionManager->ComputeTimeOut();

		// ICI!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	// Let's unregister the dialogs.
	dialogManager->RemoveAndDeleteFactory("STNO");
	// Let's unregister the connection managing messages.
	RemoveMessageHandler(STCO_MESSAGEID);
	RemoveMessageHandler(HELO_MESSAGEID);
	RemoveMessageHandler(CORE_MESSAGEID);
	RemoveMessageHandler(COAC_MESSAGEID);

    autoDestroyDone = true;
}

void KDp2p_P2PEngine::StopThread()
{
    autoDestroyDone = false;
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

void KDp2p_P2PEngine::HandleMessage(KDp2p_Message *message, int id)
{
    if (id == PING_MESSAGEID)
        ProcessPing(message);
}

void *KDp2p_P2PEngine::GetUniqueObject(int id)
{
	return uniqueObjects[id];
}

void KDp2p_P2PEngine::SetUniqueObject(int id, void *object)
{
	uniqueObjects[id] = object;
}

void KDp2p_P2PEngine::SetPeerListFileName(const string &peerListFileName)
{
	this->peerListFileName = peerListFileName;
}
