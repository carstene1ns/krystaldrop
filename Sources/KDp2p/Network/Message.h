#ifndef Message_H
#define Message_H

#include "../Tools/defines.h"
#include <SDL/SDL_net.h>

#include <vector>
#include <string>
using namespace std;

class KDp2p_Packet;
class KDp2p_NetworkAddress;
class KDp2p_UDPSocket;

/**
	A Message to send (or a message received).
*/
class DllExport KDp2p_Message
{
	/// Number of the packet sent
	static int lastMsgId;

	/// Id of the packet
	int msgId;

	vector<KDp2p_Packet*> packets;

	/**
		The destination to send the message to (if the message is to be sent)
		or the sender of the message (if the message has been received).
		The pointer is deleted when the class is destroyed.
	*/
	KDp2p_NetworkAddress *address;


	//////////////// READ VARIABLES /////////////////
	/// Position for reading the message (position in the current packet)
	int currentPos;
	/// Packet currently read
	int currentPacket;

	/**
		Date when the message has been added to the queue (if the packet is to be sent)
		or last date when a packet has been received for the message.
	*/
	int date;

	/**
		Number of packets currently received for this message.
	*/
	int nbPacketsCurrentlyReceived;

public:
	/// Creates an empty message
	KDp2p_Message();

	/**
		Creates a message from one packet that has been received.
		Other packets should be added later.
		This suppose the message is correct and that check-sum has been validated.
	*/
	KDp2p_Message(KDp2p_Packet *packet);

	/// Deletes a message
	virtual ~KDp2p_Message();

	/// Adds any data to the message
	void AddData(const void *data, int length);

	void AddInt(int value);
	void AddShortInt(short int value);
	void AddChar(char value);
	void AddFloat(float value);
	void AddDouble(double value);
	void AddString(const string &value);

	void GetData(void *data, int length);
	char GetByte();
	int GetInt();
	short int GetShortInt();
	float GetFloat();
	double GetDouble();
	string GetString();

	virtual bool isReplacing(KDp2p_Message *message);

	void UDPSend(KDp2p_UDPSocket *socket, KDp2p_NetworkAddress *address);

	/**
		Add the given package to a message being received.
		Returns true if the message is full, false otherwise.
	*/
	bool AddReceivedPacket(KDp2p_Packet *packet);

	void SetAddress(KDp2p_NetworkAddress address);
	KDp2p_NetworkAddress *GetAddress();

	int GetMsgId();

	/**
		Set the date of the message to the current date
	*/
	void UpdateTime();

	/**
		Returns the time of the message
	*/
	int GetTime();
};

#endif
