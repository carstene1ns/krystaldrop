#include "Message.h"

#include "Packet.h"
#include "../Tools/NetException.h"

#include "NetworkAddress.h"

#include <SDL/SDL.h>

int KDp2p_Message::lastMsgId = 0;

KDp2p_Message::KDp2p_Message()
{
	msgId = lastMsgId;
	lastMsgId++;
	packets.push_back(new KDp2p_Packet(msgId, 0, 1));

	currentPos = 0;
	currentPacket = 0;

	nbPacketsCurrentlyReceived = 0;

	address = 0;

	UpdateTime();
}

KDp2p_Message::KDp2p_Message(KDp2p_Packet *packet)
{
	
	currentPos = 0;
	currentPacket = 0;
	address = 0;

	//packets.reserve(packet->GetTotalPacketNumber());
	packets.resize(packet->GetTotalPacketNumber(), NULL);
	nbPacketsCurrentlyReceived = 0;
	AddReceivedPacket(packet);
	// Copy constructor
	address = new KDp2p_NetworkAddress(*(packet->GetAddress()));

	msgId = packet->GetMessageID();

	UpdateTime();
}

KDp2p_Message::~KDp2p_Message()
{
	for (unsigned int i=0; i<packets.size(); i++)
		delete packets[i];

	if (address)
		delete address;
}

void KDp2p_Message::AddData(const void *data, int length)
{
	unsigned int nbpackets = (unsigned int) packets.size();
	int written = packets[nbpackets-1]->AddDataToPacket(data, length);

	// If the packet is full and we couldn't write all in it:
	if (written != length)
	{
		nbpackets++;
		packets.push_back(new KDp2p_Packet(msgId, nbpackets-1, nbpackets));
		for (int i=0; i<(int)(nbpackets-1); i++)
		{
			packets[i]->setTotalPacketNumber(nbpackets);
		}
		// Cool, a recursive function!
		AddData(((char *)data)+written, length-written);
	}
}

void KDp2p_Message::AddInt(int value)
{
	int memArray;
	SDLNet_Write32(value, &memArray);
	AddData(&memArray, 4);
}

void KDp2p_Message::AddShortInt(short int value)
{
	short int memArray;
	SDLNet_Write16(value, &memArray);
	AddData(&memArray, 2);
}

void KDp2p_Message::AddChar(char value)
{
	AddData(&value, 1);
}

void KDp2p_Message::AddFloat(float value)
{
	AddData(&value, 4);
}

void KDp2p_Message::AddDouble(double value)
{
	AddData(&value, 8);
}

void KDp2p_Message::AddString(const string &value)
{
	AddData((void *)value.c_str(), (int)value.size()+1);
}

bool KDp2p_Message::isReplacing(KDp2p_Message *message)
{
	return false;
}

void KDp2p_Message::GetData(void *data, int length)
{
	unsigned int nbpackets = (unsigned int) packets.size();
	
	int read = packets[currentPacket]->GetData(currentPos, data, length);
	currentPos += length;

	if (read!=length)
	{
		if (nbpackets-1!=currentPacket)
		{
			currentPacket++;
			currentPos = 0;
			GetData(((char*)data)+read, length-read);
		}
		else
		{
			// We are trying to read more than available!
			throw KDp2p_NetException("Warning, trying to read more data from an UDP packet than available!");
		}
	}
}

char KDp2p_Message::GetByte()
{
	int byte = packets[currentPacket]->GetByte(currentPos);
	currentPos++;

	if (byte == -1)
	{
		if ((unsigned int) packets.size()-1!=currentPacket)
		{
			currentPacket++;
			currentPos = 0;
			byte = GetByte();
		}
		else
		{
			throw KDp2p_NetException("Warning, trying to read more data from an UDP packet than available!");
		}
	}

	return byte;
}

int KDp2p_Message::GetInt()
{
	int data;
	GetData(&data, 4);
	return SDLNet_Read32(&data);
}

short int KDp2p_Message::GetShortInt()
{
	short int data;
	GetData(&data, 2);
	return SDLNet_Read16(&data);
}

float KDp2p_Message::GetFloat()
{
	float data;
	GetData(&data, 4);
	return data;
}

double KDp2p_Message::GetDouble()
{
	double data;
	GetData(&data, 8);
	return data;
}

string KDp2p_Message::GetString()
{
	int byte;
	string str;

	do
	{
		byte = GetByte();
		str += (char)byte;
	} while (byte!=0);
	return str;
}

void KDp2p_Message::UDPSend(KDp2p_UDPSocket *socket, KDp2p_NetworkAddress *address)
{
	for (int i=0; i<(int)packets.size(); i++)
	{
		packets[i]->UDPSend(socket, address);
	}
}

bool KDp2p_Message::AddReceivedPacket(KDp2p_Packet *packet)
{
	printf("Inserted packet number : %d\n", packet->GetPacketNumber());
	packets[packet->GetPacketNumber()] = packet;
	nbPacketsCurrentlyReceived++;

	if (nbPacketsCurrentlyReceived == packets.size())
		return true;
	else
		return false;
}

void KDp2p_Message::SetAddress(KDp2p_NetworkAddress address)
{
	this->address = new KDp2p_NetworkAddress(address);
	//this->address = address;
}

KDp2p_NetworkAddress *KDp2p_Message::GetAddress()
{
	return address;
}

int KDp2p_Message::GetMsgId()
{
	return msgId;
}

void KDp2p_Message::UpdateTime()
{
	date = SDL_GetTicks();
}

int KDp2p_Message::GetTime()
{
	return date;
}
