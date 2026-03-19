#ifndef MessageHandler_H
#define MessageHandler_H

#include "../Tools/defines.h"

#include <string>
using namespace std;

class KDp2p_Message;

/**
	Abstract class (well... interface in fact) handling the messages received.
*/
class DllExport KDp2p_MessageHandler
{
public:
	KDp2p_MessageHandler();
	virtual ~KDp2p_MessageHandler();

	/**
		Methods that handle the messages received. Virtual method.
	*/
	virtual void HandleMessage(KDp2p_Message *message, int id)=0;
};

#endif
