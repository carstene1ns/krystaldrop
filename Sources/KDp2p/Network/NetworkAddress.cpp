#include "NetworkAddress.h"

#include "../Tools/NetException.h"

KDp2p_NetworkAddress::KDp2p_NetworkAddress()
{
	sdlAddress.host = 0;
	sdlAddress.port = 0;
}

KDp2p_NetworkAddress::KDp2p_NetworkAddress(unsigned int IP, unsigned short int port)
{
	sdlAddress.host = IP;
	sdlAddress.port = port;
}

KDp2p_NetworkAddress::KDp2p_NetworkAddress(unsigned char IP1, unsigned char IP2, unsigned char IP3, unsigned char IP4, unsigned short int port)
{
	sdlAddress.host = ((unsigned int)IP1) + (((unsigned int)IP2)<<8) + (((unsigned int)IP3)<<16) + (((unsigned int)IP4)<<24);
	sdlAddress.port = (port>>8) + ((port&255)<<8);
}

KDp2p_NetworkAddress::KDp2p_NetworkAddress(const KDp2p_NetworkAddress &address)
{
	sdlAddress.host = address.sdlAddress.host;
	sdlAddress.port = address.sdlAddress.port;
}

KDp2p_NetworkAddress& KDp2p_NetworkAddress::operator = (const KDp2p_NetworkAddress& address)
{
	sdlAddress.host = address.sdlAddress.host;
	sdlAddress.port = address.sdlAddress.port;
	/*if (&address != this)
	{
		this->KDp2p_NetworkAddress::~KDp2p_NetworkAddress();
		::new (this) KDp2p_NetworkAddress(address);
	}*/
	return *this;
}

bool KDp2p_NetworkAddress::operator == (const KDp2p_NetworkAddress &address) const
{
	if (address.sdlAddress.host == sdlAddress.host && address.sdlAddress.port == sdlAddress.port)
		return true;
	else
		return false;
}

bool KDp2p_NetworkAddress::operator < (const KDp2p_NetworkAddress &address) const
{
	if (address.sdlAddress.host > sdlAddress.host)
		return true;
	else if (address.sdlAddress.host == sdlAddress.host && address.sdlAddress.port > sdlAddress.port)
		return true;
	else
		return false;
}

KDp2p_NetworkAddress::~KDp2p_NetworkAddress()
{

}

unsigned int KDp2p_NetworkAddress::getIP()
{
	return sdlAddress.host;
}
	
unsigned short int KDp2p_NetworkAddress::getPort()
{
	return sdlAddress.port;
}

void KDp2p_NetworkAddress::setIP(unsigned int ip)
{
	sdlAddress.host = ip;
}

void KDp2p_NetworkAddress::setPort(unsigned short int port)
{
	sdlAddress.port = port;
}

string KDp2p_NetworkAddress::ToString()
{
	char str[22];
	int host = sdlAddress.host;
	unsigned char h4 = host>>24;
	unsigned char h3 = (host>>16) & 255;
	unsigned char h2 = (host>>8) & 255;
	unsigned char h1 = host & 255;

	unsigned short int myPort = ((sdlAddress.port)>>8) + ((sdlAddress.port&255)<<8);

	sprintf(str,"%u.%u.%u.%u:%u", h1, h2, h3, h4, myPort);
	return str;
}

bool KDp2p_NetworkAddress::FindURL(const string &url, unsigned short int port)
{
	int ret = SDLNet_ResolveHost(&sdlAddress, url.c_str(), port);
	return (ret==0);
}

void KDp2p_NetworkAddress::WriteToFile(FILE *fpt)
{
	int ret1 = fwrite(&(sdlAddress.host), sizeof(unsigned int), 1, fpt);
	int ret2 = fwrite(&(sdlAddress.port), sizeof(unsigned short int), 1, fpt);

	if (ret1==0 || ret2==0)
		throw KDp2p_NetException("Unable to write IP address to disk!");
}

void KDp2p_NetworkAddress::ReadFromFile(FILE *fpt)
{
	int ret1 = fread(&(sdlAddress.host), sizeof(unsigned int), 1, fpt);
	int ret2 = fread(&(sdlAddress.port), sizeof(unsigned short int), 1, fpt);

	if (ret1==0 || ret2==0)
		throw KDp2p_NetException("Unable to read IP address from file!");
}
