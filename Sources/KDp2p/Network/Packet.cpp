#include "Packet.h"

#include "Network.h"
#include "UDPSocket.h"
#include "NetworkAddress.h"
#include "../Tools/NetException.h"

#include <memory.h>

KDp2p_Packet::KDp2p_Packet()
{
	packet = SDLNet_AllocPacket(KDp2p_Network::getNetwork()->getPacketMaxSize());
	address = 0;
}

KDp2p_Packet::KDp2p_Packet(unsigned int msgId, unsigned int no, unsigned int nb)
{
	packet = SDLNet_AllocPacket(KDp2p_Network::getNetwork()->getPacketMaxSize());
	
	SDLNet_Write32(msgId, &(packet->data[0]));
	SDLNet_Write32(nb, &(packet->data[4]));
	SDLNet_Write32(no, &(packet->data[8]));
	SDLNet_Write32(0, &(packet->data[12]));

	packet->len = PACKET_HEADER_SIZE;
	address = 0;
}

KDp2p_Packet::~KDp2p_Packet()
{
	SDLNet_FreePacket(packet);
	if (address != 0)
		delete address;
}

int KDp2p_Packet::AddDataToPacket(const void *data, int length)
{
	if (packet->maxlen - packet->len < length)
	{
		memcpy(packet->data + packet->len, data, packet->maxlen - packet->len);
		int written = packet->maxlen - packet->len;
		packet->len = packet->maxlen;
		return written;
	}
	else
	{
		memcpy(packet->data + packet->len, data, length);
		packet->len += length;
		return length;
	}
}

/*UDPpacket *KDp2p_Packet::getPacket()
{
	SDLNet_Write32(computeCheckSum(), &(packet->data[12]));
	return packet;
}*/


unsigned int KDp2p_Packet::computeCheckSum()
{
	unsigned int sum=0;

	// compute the sum for all the data, 32 bits by 32 bits
	int i;
	for (i=PACKET_HEADER_SIZE; i<packet->len-((packet->len-PACKET_HEADER_SIZE)%4); i+=4)
	{
		unsigned int number;
		memcpy(&number, &(packet->data[i]), sizeof(unsigned int));
		sum += SDLNet_Read32(&number);
		//sum += (unsigned int)(packet->data[i]);
	}

	// what about the few remaining bits?
	if ((packet->len-PACKET_HEADER_SIZE)%4 == 0)
		return sum;

	unsigned int sup = 0;
	memcpy(&sup, &packet->data[i], packet->len%4);
	sum += SDLNet_Read32(&sup);

	return sum;
}

void KDp2p_Packet::setTotalPacketNumber(unsigned int nbPackets)
{
	SDLNet_Write32(nbPackets, &(packet->data[4]));
}

unsigned int KDp2p_Packet::GetMessageID()
{
	return SDLNet_Read32(&(packet->data[0]));
}

unsigned int KDp2p_Packet::GetTotalPacketNumber()
{
	return SDLNet_Read32(&(packet->data[4]));
}

unsigned int KDp2p_Packet::GetPacketNumber()
{
	return SDLNet_Read32(&(packet->data[8]));
}

int KDp2p_Packet::GetData(int pos, void *data, int length)
{
	if (packet->maxlen - (pos+PACKET_HEADER_SIZE) < length)
	{
		memcpy(data, packet->data + pos + PACKET_HEADER_SIZE, packet->maxlen - (pos+PACKET_HEADER_SIZE));
		return packet->maxlen - (pos+PACKET_HEADER_SIZE);
	}
	else
	{
		memcpy(data, packet->data + pos + PACKET_HEADER_SIZE, length);
		return length;
	}
}

int KDp2p_Packet::GetByte(int pos)
{
	//pos += PACKET_HEADER_SIZE;
	pos += 1;
	if (pos + PACKET_HEADER_SIZE >= packet->maxlen)
		return 256;
	else
		return ((char *)(packet->data))[pos + PACKET_HEADER_SIZE - 1];
}

void KDp2p_Packet::UDPSend(KDp2p_UDPSocket *socket, KDp2p_NetworkAddress *address)
{
	packet->address.host = address->getIP();
	packet->address.port = address->getPort();
	SDLNet_Write32(computeCheckSum(), &(packet->data[12]));
	int ret = SDLNet_UDP_Send(socket->udpsock, -1, packet);
	//printf("Sent packet %u\n", GetPacketNumber());
	if (ret == 0)
		throw KDp2p_NetException("Unable to send packet");
	// Here to give the hand to the other processes (especially the receive process when tests are made locally).
	SDL_Delay(1);
}

bool KDp2p_Packet::UDPReceive(KDp2p_UDPSocket *socket)
{
	int ret = SDLNet_UDP_Recv(socket->udpsock , packet);

	if (ret == 0)
		return false;

	if (address)
		delete address;

	address = new KDp2p_NetworkAddress();
	address->setIP(packet->address.host);
	address->setPort(packet->address.port);

	int declaredCheckSum = SDLNet_Read32(&(packet->data[12]));

	// If checkSum is wrong
	if (computeCheckSum() != declaredCheckSum)
		throw KDp2p_NetException("Warning! Checksum verification failed!");

	return true;
}

KDp2p_NetworkAddress *KDp2p_Packet::GetAddress()
{
	return address;
}
