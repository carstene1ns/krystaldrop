#ifndef SerializableString_H
#define SerializableString_H

#include "../../Tools/defines.h"

#include "SerializableObject.h"

#include <string>
using namespace std;

/**
	A string that can be serialized and sent over the network
*/
class DllExport KDp2p_SerializableString : public KDp2p_SerializableObject
{
protected:
	string str;

public:

	KDp2p_SerializableString(const string &_str);

	virtual ~KDp2p_SerializableString();

	/**
		Add the object to a Message
	*/
	virtual void AddToMessage(KDp2p_Message *message);

	/**
		Read the object from a Message
	*/
	virtual void ReadFromMessage(KDp2p_Message *message);

	/**
		Sets the string
	*/
	void SetString(const string &str);

	/**
		Gets the string
	*/
	string GetString();
};

#endif
