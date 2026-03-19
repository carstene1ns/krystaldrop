#ifndef P2PId_H
#define P2PId_H

#include "../Tools/defines.h"

#include <string>
using namespace std;

class KDp2p_Message;

/**
	The Id of a comuter on the network coded on 128 bits
	The first 32 bits are reserved for the GAME played ID.
*/
class DllExport KDp2p_P2PId
{
	friend class KDp2p_BPosition;
	int id[4];

public:
	KDp2p_P2PId();
	~KDp2p_P2PId();

	int GetNbBits();
	bool GetBit(int number);

	void PickRandomId(int gameId);

	void AddToMessage(KDp2p_Message *message);
	void ReadFromMessage(KDp2p_Message *message);

	string ToString();
};

#endif
