#ifndef NetException_H
#define NetException_H

#include "defines.h"

#include <string>
using namespace std;

/**
	A network exception.
*/
class DllExport KDp2p_NetException
{
	string errStr;

public:
	KDp2p_NetException();
	KDp2p_NetException(const string &errStr);
	//KDp2p_NetException(const string &errStr, ...);
	virtual ~KDp2p_NetException();

	virtual string ToString() const;
	

};

#endif
