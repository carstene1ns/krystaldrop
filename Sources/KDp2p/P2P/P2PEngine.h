#ifndef P2PEngine_H
#define P2PEngine_H

#include "../Tools/defines.h"

#include "../Tools/Thread.h"

#include "P2PId.h"

class KDp2p_AllPeers;
class KDp2p_Network;
class KDp2p_UDPSocket;
class KDp2p_SentMessageQueue;
class KDp2p_ReceivedMessageQueue;
class KDp2p_NetworkAddress;
class KDp2p_Message;
class KDp2p_ConnectionManager;
class KDp2p_ConnectionListener;
class KDp2p_DialogManager;

/**
	Contains the list of all peers ever contacted, with the last date when they have been contacted
*/
class DllExport KDp2p_P2PEngine : public KDp2p_Thread
{
	static int maxPeerNumberInFile;
	static int engineVersion;
	static int peerFileVersion;

	KDp2p_AllPeers *peersList;
	KDp2p_Network *network;
	KDp2p_UDPSocket *socket;
	KDp2p_SentMessageQueue *sendQueue;
	KDp2p_ReceivedMessageQueue *recvQueue;
	KDp2p_ConnectionManager *connectionManager;
	KDp2p_DialogManager *dialogManager;

	KDp2p_P2PId myId;

	enum KDp2p_EngineState
	{
		stopped = 0,
		lookingForPeer,
		failedToFindPeer,
		connected
	};

	KDp2p_EngineState state;

	/**
		Time in milliseconds after which the connection is supposed as lost.
		Default is 10 seconds
	*/
	unsigned int connectionTimeOut;

	/**
		Time in milliseconds after which we should send an update message to keep the connexion alove.
		This should be smaller than connectionTimeOut, ideally = (connectionTimeOut-ping-eps)/2
		Default is 4 seconds
	*/
	unsigned int connectionKeepAliveTime;

	/**
		Time in milliseconds between each checking of the timeout for connections.
		Default is 1 s
	*/
	unsigned int checkTimeOutInterval;

	/**
		Time of the next CheckTimeOut.
	*/
	unsigned int nextCheckTimeOutTime;

	/**
		Destroys the thread when set to true
	*/
	bool autoDestroy;

	/**
		Set to true when the autodestruction is done
	*/
	bool autoDestroyDone;

	
	
public:
	KDp2p_P2PEngine();
	virtual ~KDp2p_P2PEngine();

	static int getMaxPeerNumberInFile();
	static int getEngineVersion();
	static int getPeerFileVersion();

	void Init(int gameId, short int port = 12354);
	void Close();

	/**
		Sends a ping message to the given network address, waiting for a pong.
	*/
	void Ping(KDp2p_NetworkAddress *address);

	/**
		Process the received messages
	*/
	void ProcessMessages();

	/**
		Sends the pong message and adds the ping sender to the AllPeers list.
	*/
	void ProcessPing(KDp2p_Message *message);

	/**
		Receives the pong message. If the message is the first png message received, starts the binary tree.
	*/
	void ProcessPong(KDp2p_Message *message);

	/**
		Returns the SendMessageQueue
	*/
	KDp2p_SentMessageQueue *GetSendQueue();

	/**
		Returns the ReceiveMessageQueue
	*/
	KDp2p_ReceivedMessageQueue *GetRecvQueue();

	/**
		Returns the DialogManager
	*/
	KDp2p_DialogManager *GetDialogManager();

	/**
		Returns the time in millisecond after which the connection is supposed as lost.
		Default is 10 seconds
	*/
	int GetConnectionTimeOut();

	/**
		Returns the time in millisecond after which we should send an update message to keep the connexion alove.
		This should be smaller than connectionTimeOut, ideally = (connectionTimeOut-ping-eps)/2
		Default is 4 seconds
	*/
	int GetConnectionKeepAliveTime();
	
	/**
		Connects to a computer
	*/
	void Connect(KDp2p_NetworkAddress *address, KDp2p_ConnectionListener *listener);

	/**
		Runs the thread
	*/
	void Run();

	/**
		Asks the thread to stop
	*/
	void StopThread();
};

#endif
