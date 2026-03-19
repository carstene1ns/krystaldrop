#include "SerializableString.h"

#include "../../Network/Message.h"

KDp2p_SerializableString::KDp2p_SerializableString(const string &_str) : KDp2p_SerializableObject(), str(_str)
{

}

KDp2p_SerializableString::~KDp2p_SerializableString()
{

}

void KDp2p_SerializableString::AddToMessage(KDp2p_Message *message)
{
	message->AddString(str);
}

void KDp2p_SerializableString::ReadFromMessage(KDp2p_Message *message)
{
	str = message->GetString();
}

void KDp2p_SerializableString::SetString(const string &str)
{
	this->str = str;
}

string KDp2p_SerializableString::GetString()
{
	return str;
}
