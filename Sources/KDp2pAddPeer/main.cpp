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

#include "../KDp2p/Tools/NetException.h"

#include <string>
using namespace std;

int main (int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	if (argc != 3)
	{
		printf("KDp2pAddPeer Syntax:\nKDp2pAddPeer file IP\n");
		printf("file is the name of the file containing the list of IP addresses (by default peerList.p2p)\n");
		printf("IP is the IP address added + the port in the form xxx.xxx.xxx.xxx:yyyyy\n");
		printf("if the file does not exist, it is created\n");
		exit(0);
	}

	KDp2p_AllPeers *peers = new KDp2p_AllPeers();
	try
	{
		peers->Load(argv[1]);
	}
	catch (KDp2p_NetException ex)
	{
		printf("Creating file %s\n", argv[1]);
	}

	string fullIP = argv[2];

	string number1 = fullIP.substr(0,fullIP.find('.'));
	fullIP = fullIP.substr(fullIP.find('.')+1);
	string number2 = fullIP.substr(0,fullIP.find('.'));
	fullIP = fullIP.substr(fullIP.find('.')+1);
	string number3 = fullIP.substr(0,fullIP.find('.'));
	fullIP = fullIP.substr(fullIP.find('.')+1);
	string number4 = fullIP.substr(0,fullIP.find(':'));
	string port = fullIP.substr(fullIP.find(':')+1);

	printf("%s.%s.%s.%s:%s", number1.c_str(),number2.c_str(),number3.c_str(),number4.c_str(),port.c_str());

	
	KDp2p_NetworkAddress *addr = new KDp2p_NetworkAddress(atoi(number1.c_str()),atoi(number2.c_str()),atoi(number3.c_str()),atoi(number4.c_str()),atoi(port.c_str()));
	peers->AddPeer(addr);
	delete addr;

	peers->Save(argv[1]);
	delete peers;

	SDL_Quit();
	return 0;
}
