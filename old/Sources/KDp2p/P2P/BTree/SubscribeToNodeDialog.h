#ifndef SubscribeToNodeDialog_H
#define SubscribeToNodeDialog_H

#include "../../Tools/defines.h"

#include "../Dialog.h"
#include "BPosition.h"
#include "../../Network/NetworkAddress.h"

#include <deque>
using namespace std;

class KDp2p_SerializableObject;

#define SUSCRIBETONODE_MESSAGEID ((((('S'<<24) + ('T'<<16) + ('N'<<8) + 'O'))))

/**
	The SubscribeToNodeDialog sends a request message to get the details of a node.
	The receiver will suscribe the sender in its distributed binary tree and send back
	the details of the asked node.
*/
class DllExport KDp2p_SubscribeToNodeDialog : public KDp2p_Dialog
{
#define SUPER KDp2p_Dialog
	/// The node we want details from.
	KDp2p_BPosition nodePos;

	/**
		The node we want to suscribe to.
		We want the peer to send us the details of any nodes between
		nodePos and nodeGoal he has the knowledge of.
	*/
	KDp2p_BPosition goalPos;

	/// The tree we want to subscribe to
	unsigned int treeID;

	/// The object we want to add to the tree
	KDp2p_SerializableObject *object;

	/**
		A list of alternative peers that can answer the question
		Used by the caller in case the first asked node would be unable to answer
	*/
	deque<KDp2p_NetworkAddress> alternativePeers;

public:
	/// Constructor used by the SubscribeToNode factory when sending the message
	KDp2p_SubscribeToNodeDialog(KDp2p_P2PEngine *engine);
	
	/// Constructor used by the SubscribeToNode factory when receiving the message
	KDp2p_SubscribeToNodeDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message, int questionType);

	virtual ~KDp2p_SubscribeToNodeDialog();

	/**
		Method called when the answer is longer than timeOut to arrive.
		We try to ask the question to alternativePeers.
		If there are no alternativePeers anymore, we add the object directly into our DBT
	*/
	virtual void OnTimeOut();

	/// Method called on reception of the answer
	virtual void OnAnswer();

	/// Method called on reception of the question
	virtual void OnQuestion(KDp2p_DialogFactory *factory);

	virtual void SendQuestion();

	/**
		Sets the nodes to retrieve (used by the caller just before sending the question)
		pos is the node from which we want details, goal is the node we want to susbribe to.
		pos should therefore be a grandfather of goal
	*/
	void SetNodeToRetrieve(unsigned int treeID, const KDp2p_BPosition &pos, const KDp2p_BPosition &goal, KDp2p_SerializableObject *object);

	/**
		Adds an alternative peer to the list of potential answerers
	*/
	void AddAlternativePeer(const KDp2p_NetworkAddress &alternativePeer);

	/**
		Asks the question again to one of the alternative peers.
		Note: if the configuration of the tree has changed, the question is adapted.
	*/
	void AskAlternativePeer();
};

#endif
