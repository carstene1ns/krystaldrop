#ifndef PingCounter_H
#define PingCounter_H

#include "../Tools/defines.h"
#include "../Tools/Mutex.h"
#include "../Network/NetworkAddress.h"

#include <deque>
using namespace std;

/**
	A class to count the pings (+1 when a question is asked, -1 when an answer or a timeout is received
*/
class DllExport KDp2p_PingCounter
{
	int counter;
	deque<KDp2p_NetworkAddress> pingedAddresses;
	deque<KDp2p_NetworkAddress>::iterator it;

	KDp2p_Mutex mutex;

	
public:
	KDp2p_PingCounter();
	virtual ~KDp2p_PingCounter();

	/// Method called when a ping is asked
	virtual void OnQuestion();

	/// Method called when a ping answer times out.
	virtual void OnTimeOut();

	/// ethod called when a ping is answered.
	virtual void OnAnswer(KDp2p_NetworkAddress *address);

	/// Returns the number of pings non-answered
	int GetPingCounter();

	/// Returns the current pointed address
	KDp2p_NetworkAddress GetAddress();

	/// Points to the next address in the list. Returns flase if there is no next address.
	bool NextAddress();

	/// Resets the address list to the beginning.
	void ResetAddressesPointer();
};

#endif
