#include <SDL/SDL.h>

#include "../KDp2p/Network/Network.h"
#include "../KDp2p/Network/UDPSocket.h"
#include "../KDp2p/Network/SentMessageQueue.h"
#include "../KDp2p/Network/ReceivedMessageQueue.h"
#include "../KDp2p/Network/Message.h"
#include "../KDp2p/Network/NetworkAddress.h"

#include "../KDp2p/P2P/AllPeers.h"

#include "../KDp2p/P2P/P2PEngine.h"

#include "../KDp2p/P2P/P2PId.h"

#include <string>
using namespace std;

int main (int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	KDp2p_P2PEngine *engine = new KDp2p_P2PEngine();
	// Init with a krystaldrop code
	engine->Init(719719);
	KDp2p_NetworkAddress *addr = new KDp2p_NetworkAddress(192,168,0,1,12354);
	//KDp2p_NetworkAddress *addr = new KDp2p_NetworkAddress();
	//addr->FindURL("127.0.0.1",12354);
	engine->Ping(addr);
	delete addr;

	SDL_Delay(50000);

	engine->Close();
	delete engine;

	/*KDp2p_Network *myNetwork = KDp2p_Network::initNetwork();

	KDp2p_UDPSocket *socket = new KDp2p_UDPSocket(12354);

	KDp2p_SentMessageQueue *sendQueue = new KDp2p_SentMessageQueue(socket);
	KDp2p_ReceivedMessageQueue *recvQueue = new KDp2p_ReceivedMessageQueue(socket);

	KDp2p_Message *message = new KDp2p_Message();
	for (int i=0; i<1000; i++)
	{
		message->AddInt(i);
		message->AddInt(42);
		message->AddString("Hello World!");
	}
	//KDp2p_NetworkAddress *addr = new KDp2p_NetworkAddress();
	//addr->FindURL("localhost",12354);
	//message->SetAddress(addr);
	message->SetAddress(KDp2p_NetworkAddress(127,0,0,1,12354));
	
	sendQueue->AddMessage(message);

	SDL_Delay(10000);

	KDp2p_Message *recvMessage = recvQueue->PeekMessage();
	if (recvMessage == 0)
		printf("No messages received!!!!!\n");
	else
	{
		for (int i=0; i<1000; i++)
		{
			unsigned int i1 =  recvMessage->GetInt();
			unsigned int i2 =  recvMessage->GetInt();
			string str = recvMessage->GetString();
			printf("First int: %d\n Second Int: %d\nString: %s\n", i1, i2, str.c_str());
		}
		printf("Addresse du message: %s\n", recvMessage->GetAddress()->ToString().c_str());
		delete recvMessage;
	}


	delete sendQueue;
	delete recvQueue;

	delete socket;

	myNetwork->closeNetwork();*/

	/*KDp2p_AllPeers *peers = new KDp2p_AllPeers();
	KDp2p_NetworkAddress *addr3 = new KDp2p_NetworkAddress(192,168,0,1,12354);
	peers->AddPeer(addr3);
	delete addr3;
	SDL_Delay(1000);
	//addr3 = new KDp2p_NetworkAddress(12,23,34,45,502);
	//peers->AddPeer(addr3);
	//delete addr3;
	//SDL_Delay(1000);
	//addr3 = new KDp2p_NetworkAddress(54,43,32,21,502);
	//peers->AddPeer(addr3);
	//delete addr3;
//	peers->Load("peerList.p2p");

	KDp2p_NetworkAddress addr4;
	addr4 = peers->GetPeer();
	printf("%s\n",addr4.ToString().c_str());
	peers->Next();
	addr4 = peers->GetPeer();
	printf("%s\n",addr4.ToString().c_str());
	peers->Next();
	addr4 = peers->GetPeer();
	printf("%s\n",addr4.ToString().c_str());
	peers->Next();

	peers->Save("peerList.p2p");
*/
	printf("YO!");

	SDL_Quit();
	return 0;
}
