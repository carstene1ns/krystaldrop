#include "NetworkAddress.h"

#include "../Tools/NetException.h"
#include "Message.h"

KDp2p_NetworkAddress::KDp2p_NetworkAddress()
{
	/*sdlAddress.host = 0;
	sdlAddress.port = 0;*/

	myAddr.sin_family = AF_INET;
	myAddr.sin_port = 0;
	myAddr.sin_addr.S_un.S_addr = 0;
	memset(&(myAddr.sin_zero), 0, 8); // zero the rest of the struct
}

KDp2p_NetworkAddress::KDp2p_NetworkAddress(unsigned int IP, unsigned short int port)
{
	/*sdlAddress.host = IP;
	sdlAddress.port = port;*/

	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	myAddr.sin_addr.S_un.S_addr = htonl(IP);
	memset(&(myAddr.sin_zero), 0, 8); // zero the rest of the struct
}

KDp2p_NetworkAddress::KDp2p_NetworkAddress(unsigned char IP1, unsigned char IP2, unsigned char IP3, unsigned char IP4, unsigned short int port)
{
	/*sdlAddress.host = ((unsigned int)IP1) + (((unsigned int)IP2)<<8) + (((unsigned int)IP3)<<16) + (((unsigned int)IP4)<<24);
	sdlAddress.port = (port>>8) + ((port&255)<<8);*/

	// A TESTER!
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = ((unsigned int)IP1) + (((unsigned int)IP2)<<8) + (((unsigned int)IP3)<<16) + (((unsigned int)IP4)<<24);
	myAddr.sin_addr.S_un.S_addr = htons(port);
	memset(&(myAddr.sin_zero), 0, 8); // zero the rest of the struct
}

KDp2p_NetworkAddress::KDp2p_NetworkAddress(const KDp2p_NetworkAddress &address)
{
	/*sdlAddress.host = address.sdlAddress.host;
	sdlAddress.port = address.sdlAddress.port;*/

	myAddr.sin_family = address.myAddr.sin_family;
	myAddr.sin_port = address.myAddr.sin_port;
	myAddr.sin_addr.S_un.S_addr = address.myAddr.sin_addr.S_un.S_addr;
	memset(&(myAddr.sin_zero), 0, 8); // zero the rest of the struct (or should we copy it?)
}

KDp2p_NetworkAddress& KDp2p_NetworkAddress::operator = (const KDp2p_NetworkAddress& address)
{
	/*sdlAddress.host = address.sdlAddress.host;
	sdlAddress.port = address.sdlAddress.port;*/
	
	myAddr.sin_family = address.myAddr.sin_family;
	myAddr.sin_port = address.myAddr.sin_port;
	myAddr.sin_addr.S_un.S_addr = address.myAddr.sin_addr.S_un.S_addr;
	memset(&(myAddr.sin_zero), 0, 8); // zero the rest of the struct (or should we copy it?)

	return *this;
}

bool KDp2p_NetworkAddress::operator == (const KDp2p_NetworkAddress &address) const
{
	if (myAddr.sin_addr.S_un.S_addr == address.myAddr.sin_addr.S_un.S_addr && myAddr.sin_family == address.myAddr.sin_family && myAddr.sin_port == address.myAddr.sin_port)
		return true;
	else
		return false;

	/*if (address.sdlAddress.host == sdlAddress.host && address.sdlAddress.port == sdlAddress.port)
		return true;
	else
		return false;*/
}

bool KDp2p_NetworkAddress::operator < (const KDp2p_NetworkAddress &address) const
{
	if (address.myAddr.sin_addr.S_un.S_addr > myAddr.sin_addr.S_un.S_addr)
		return true;
	else if (address.myAddr.sin_addr.S_un.S_addr == myAddr.sin_addr.S_un.S_addr && address.myAddr.sin_port > myAddr.sin_port)
		return true;
	else if (address.myAddr.sin_addr.S_un.S_addr == myAddr.sin_addr.S_un.S_addr && address.myAddr.sin_port == myAddr.sin_port && address.myAddr.sin_family > myAddr.sin_family)
		return true;
	else
		return false;

	/*if (address.sdlAddress.host > sdlAddress.host)
		return true;
	else if (address.sdlAddress.host == sdlAddress.host && address.sdlAddress.port > sdlAddress.port)
		return true;
	else
		return false;*/
}

KDp2p_NetworkAddress::~KDp2p_NetworkAddress()
{

}

unsigned int KDp2p_NetworkAddress::getIP()
{
	return myAddr.sin_addr.S_un.S_addr;
	//return sdlAddress.host;
}
	
unsigned short int KDp2p_NetworkAddress::getPort()
{
	return myAddr.sin_port;
	//return sdlAddress.port;
}

void KDp2p_NetworkAddress::setIP(unsigned int ip)
{
	myAddr.sin_addr.S_un.S_addr = ip;
	//sdlAddress.host = ip;
}

void KDp2p_NetworkAddress::setPort(unsigned short int port)
{
	myAddr.sin_port = port;
	//sdlAddress.port = port;
}

string KDp2p_NetworkAddress::ToString() const
{
	char str[22];

	sprintf(str,"%s:%u", inet_ntoa(myAddr.sin_addr), ntohs(myAddr.sin_port));
	return str;


	/*char str[22];
	int host = sdlAddress.host;
	unsigned char h4 = host>>24;
	unsigned char h3 = (host>>16) & 255;
	unsigned char h2 = (host>>8) & 255;
	unsigned char h1 = host & 255;

	unsigned short int myPort = ((sdlAddress.port)>>8) + ((sdlAddress.port&255)<<8);

	sprintf(str,"%u.%u.%u.%u:%u", h1, h2, h3, h4, myPort);
	return str;*/
}

bool KDp2p_NetworkAddress::FindURL(const string &url, unsigned short int port)
{
	hostent *h;
	h = gethostbyname(url.c_str());

	if (h == 0)
	{
		return false;
	}

	myAddr.sin_addr = *((in_addr *) h->h_addr_list[0]);
	myAddr.sin_port = htons(port);
	return true;

/*	int ret = SDLNet_ResolveHost(&sdlAddress, url.c_str(), port);
	return (ret==0);*/
}

void KDp2p_NetworkAddress::WriteToFile(FILE *fpt)
{
	// data is written in network byte order
	int ret1 = fwrite(&(myAddr.sin_addr.S_un.S_addr), sizeof(unsigned int), 1, fpt);
	int ret2 = fwrite(&(myAddr.sin_port), sizeof(unsigned short int), 1, fpt);

	//int ret1 = fwrite(&(sdlAddress.host), sizeof(unsigned int), 1, fpt);
	//int ret2 = fwrite(&(sdlAddress.port), sizeof(unsigned short int), 1, fpt);

	if (ret1==0 || ret2==0)
		throw KDp2p_NetException("Unable to write IP address to disk!");
}

void KDp2p_NetworkAddress::ReadFromFile(FILE *fpt)
{
	int ret1 = fread(&(myAddr.sin_addr.S_un.S_addr), sizeof(unsigned int), 1, fpt);
	int ret2 = fread(&(myAddr.sin_port), sizeof(unsigned short int), 1, fpt);

//	int ret1 = fread(&(sdlAddress.host), sizeof(unsigned int), 1, fpt);
//	int ret2 = fread(&(sdlAddress.port), sizeof(unsigned short int), 1, fpt);

	if (ret1==0 || ret2==0)
		throw KDp2p_NetException("Unable to read IP address from file!");
}

void KDp2p_NetworkAddress::AddToMessage(KDp2p_Message *message)
{
	message->AddInt(myAddr.sin_addr.S_un.S_addr);
	message->AddShortInt(myAddr.sin_port);

	//message->AddInt(sdlAddress.host);
	//message->AddShortInt(sdlAddress.port);
}

void KDp2p_NetworkAddress::ReadFromMessage(KDp2p_Message *message)
{
	myAddr.sin_addr.S_un.S_addr = message->GetInt();
	myAddr.sin_port = message->GetShortInt();

	//sdlAddress.host = message->GetInt();
	//sdlAddress.port = message->GetShortInt();
}
