#ifndef NetworkAddress_H
#define NetworkAddress_H

#include "../Tools/defines.h"

#include <SDL/SDL_net.h>

#ifdef WIN32
#include <winsock.h>
#endif

#include "stdio.h"

#include <string>
using namespace std;

class KDp2p_Message;

/**
	A network Address (IP+port)
*/
class DllExport KDp2p_NetworkAddress
{
	friend class KDp2p_Socket;

	sockaddr_in myAddr;

public:
	/**
		Default Constructor
	*/
	KDp2p_NetworkAddress();

	/**
		Constructor from int and short int
		int in host byte order, port in host byte order
	*/
	KDp2p_NetworkAddress(unsigned int IP, unsigned short int port);

	/**
		Constructor from 4 chars (IP) and short int (port)
		port must be given in host byte order
	*/
	KDp2p_NetworkAddress(unsigned char IP1, unsigned char IP2, unsigned char IP3, unsigned char IP4, unsigned short int port);

	/**
		Copy constructor
	*/
	KDp2p_NetworkAddress(const KDp2p_NetworkAddress &address);

	KDp2p_NetworkAddress& operator = (const KDp2p_NetworkAddress& address);

	bool operator == (const KDp2p_NetworkAddress &file) const;

	bool operator < (const KDp2p_NetworkAddress &file) const;

	/**
		Destructor
	*/
	~KDp2p_NetworkAddress();

	/**
		Returns the IP in network byte orders
	*/
	unsigned int getIP();
	
	/**
		Returns the port in network byte order
	*/
	unsigned short int getPort();

	/**
		Sets the IP
		The IP must be passed in Network Byte Order (use htonl to convert values)
	*/
	void setIP(unsigned int ip);
	
	/**
		Sets the port
		The port must be passed in Network Byte Order (use htons to convert values)
	*/
	void setPort(unsigned short int port);

	/**
		Returns a string containing the IP Adress and Port: x.x.x.x:p
	*/
	string ToString() const;

	/**
		Finds the address from URL
		returns true on success, false on failure (DNS error)
		port must be passed in host byte order
	*/
	bool FindURL(const string &url, unsigned short int port);

	/**
		Write the IP Address to the file in binary format
	*/
	void WriteToFile(FILE *fpt);

	/**
		Reads the IP Adress from a file
	*/
	void ReadFromFile(FILE *fpt);

	/**
		Adds the IP to the given message
	*/
	void AddToMessage(KDp2p_Message *message);

	/**
		Reads the IP from the given message
	*/
	void ReadFromMessage(KDp2p_Message *message);

};

#endif
