#include "P2PId.h"

#include "../Network/Message.h"

#include <stdlib.h>

KDp2p_P2PId::KDp2p_P2PId()
{

}

KDp2p_P2PId::~KDp2p_P2PId()
{

}

int KDp2p_P2PId::GetNbBits()
{
	return 128;
}

bool KDp2p_P2PId::GetBit(int number)
{
	return (id[number/8] & (number%8)) != 0;
}

void KDp2p_P2PId::PickRandomId(int gameId)
{
	id[0] = gameId;
	id[1] = rand();
	id[2] = rand();
	id[3] = rand();
}

void KDp2p_P2PId::AddToMessage(KDp2p_Message *message)
{
	message->AddInt(id[0]);
	message->AddInt(id[1]);
	message->AddInt(id[2]);
	message->AddInt(id[3]);
}

void KDp2p_P2PId::ReadFromMessage(KDp2p_Message *message)
{
	id[0] = message->GetInt();
	id[1] = message->GetInt();
	id[2] = message->GetInt();
	id[3] = message->GetInt();
}

string KDp2p_P2PId::ToString()
{
	char buf[33];
	sprintf(buf, "%X%X%X%X", id[0], id[1], id[2], id[3]);
	return buf;
}
