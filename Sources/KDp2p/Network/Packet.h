#ifndef Packet_H
#define Packet_H

#include "../Tools/defines.h"
#include <SDL/SDL_net.h>

class KDp2p_UDPSocket;
class KDp2p_NetworkAddress;

/**
	A Packet from a message.
*/
class DllExport KDp2p_Packet
{
protected:
#define PACKET_HEADER_SIZE 16

	UDPpacket *packet;

	unsigned int computeCheckSum();

	/// The address of the sender
	KDp2p_NetworkAddress *address;

public:
	/**
		Creates an empty packet for receiving data
	*/
	KDp2p_Packet();

	/**
		Creates an empty packet to be sent
		The packet will be the packet number no from message msgId, containing nb different packages.
	*/
	KDp2p_Packet(unsigned int msgId, unsigned int no, unsigned int nb);

	/// Deletes a packet
	virtual ~KDp2p_Packet();


	/**
		Adds data to the packet. The number of bytes actually written is returned.
	*/
	int AddDataToPacket(const void *data, int length);

	/**
		Returns the UDPpacket.
		Computes the checksum just before!
	*/
	//UDPpacket *getPacket();

	/**
		Sets the total number of packets in the message containing this packet
	*/
	void setTotalPacketNumber(unsigned int nbPackets);

	/**
		Returns the messageID of the packet
	*/
	unsigned int GetMessageID();

	/**
		Returns the total number of packets in the message containing this packet
	*/
	unsigned int  GetTotalPacketNumber();

	/**
		Returns the packet number in the message
	*/
	unsigned int GetPacketNumber();

	/**
		Returns the asked data from given position in packet.
		Pos 0 is the first byte after the header.
		Returns the number of bytes effectively retrieved.
	*/
	int GetData(int pos, void *data, int length);

	/**
		Returns the byte asked at the given position in packet.
		Pos 0 is the first byte after the header.
		If the pos is out of bounds, returns 256
	*/
	int GetByte(int pos);

	/**
		Sends the packet
	*/
	void UDPSend(KDp2p_UDPSocket *socket, KDp2p_NetworkAddress *address);

	/**
		Receives a packet
	*/
	bool UDPReceive(KDp2p_UDPSocket *socket);

	KDp2p_NetworkAddress *GetAddress();
};

#endif
