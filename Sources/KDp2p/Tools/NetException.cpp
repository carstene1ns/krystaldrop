#include "NetException.h"

#include "../../KDpp/Tools/Logfile.h"

KDp2p_NetException::KDp2p_NetException()
{

}

KDp2p_NetException::KDp2p_NetException(const string &errStr)
{
	string message = "Network Exception : ";
	message += errStr;
	KD_LogFile::printf2(message);
	this->errStr = errStr;
}

/*KDp2p_NetException::KDp2p_NetException(const string &errStr, ...)
{
	string message = "Network Exception : ";
	message += errStr;

	va_list argptr;

	va_start (argptr, errStr);
	KD_LogFile::printf2(message, argptr);
	va_end (argptr);
}*/

KDp2p_NetException::~KDp2p_NetException()
{

}

string KDp2p_NetException::ToString() const
{
	return errStr;
}
