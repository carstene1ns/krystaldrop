#ifndef SerializableObject_H
#define SerializableObject_H

#include "../../Tools/defines.h"

class KDp2p_Message;

/**
	An object that can be serialized and sent over the network
*/
class DllExport KDp2p_SerializableObject
{
protected:
	

public:

	KDp2p_SerializableObject();

	virtual ~KDp2p_SerializableObject();

	/**
		Add the object to a Message
	*/
	virtual void AddToMessage(KDp2p_Message *message)=0;

	/**
		Read the object from a Message
	*/
	virtual void ReadFromMessage(KDp2p_Message *message)=0;
};

#endif
