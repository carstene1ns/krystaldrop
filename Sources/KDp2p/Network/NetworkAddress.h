#ifndef NetworkAddress_H
#define NetworkAddress_H

#include "../Tools/defines.h"

#include <SDL/SDL_net.h>

#include "stdio.h"

#include <string>
using namespace std;

/**
	A network Address (IP+port)
  */
class DllExport KDp2p_NetworkAddress
{
	// Warning, SDL IP adresses and port are stored in reverse order (little endian?)
	IPaddress sdlAddress;

public:
	/**
		Default Constructor
	*/
	KDp2p_NetworkAddress();

	/**
		Constructor from int and short int
	*/
	KDp2p_NetworkAddress(unsigned int IP, unsigned short int port);

	/**
		Constructor from 4 chars (IP) and short int (port)
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
		Returns the IP
	*/
	unsigned int getIP();
	
	/**
		Returns the port
	*/
	unsigned short int getPort();

	/**
		Returns the IP
	*/
	void setIP(unsigned int ip);
	
	/**
		Returns the port
	*/
	void setPort(unsigned short int port);

	/**
		Returns a string containing the IP Adress and Port: x.x.x.x:p
	*/
	string ToString();

	/**
		Finds the address from URL
		returns true on success, false on failure (DNS error)
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
};

#endif
