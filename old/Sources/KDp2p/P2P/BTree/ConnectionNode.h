#ifndef ConnectionNode_H
#define ConnectionNode_H

#include "../../Tools/defines.h"

#include "BNode.h"
#include "../ConnectionListener.h"
#include "../../Network/NetworkAddress.h"

#include <deque>
using namespace std;

class KDp2p_ConnectionHandler;

/**
	A leaf in the Distributed P2P tree, used to gain access to the rest of the tree from the P2P network.
*/
class DllExport KDp2p_ConnectionNode : public KDp2p_BNode, public KDp2p_ConnectionListener
{
	/**
		The IP Adresses in this deque are possibly usable to access the rest of the tree.
	*/
	deque<KDp2p_ConnectionHandler *> connections;

	class BannedAddress
	{
	public:
		KDp2p_NetworkAddress address;
		unsigned int time;

		BannedAddress(KDp2p_NetworkAddress _address, unsigned int _time) : address(_address), time(_time) {}
	};

	/**
		The IP Addresses in this deque have been used and are NOT part of the connectionNode.
		They returned an error.
		Therefore, they should not be used, even if another peer tells us they can be.
		At least for a few seconds.

		TO DO: In V2, WE SHOULD INFORM THE NODE WHO TOLD US THE PEER HAD THAT POS THAT HE WAS WRONG.
		RIGHT NOW, WE CAN'T EVEN REMOVE AN OBJECT ONCE IT HAS BEEN ADDED!
	*/
	deque<BannedAddress> blackList;

	/**
		Clears the list of black-listed IPs by removing outdated black-listed IPs.
	*/
	void ClearBlackList();

	/**
		Removes the given connection from the node without sending any CloseConnection message.
	*/
	void RemoveConnection(const KDp2p_NetworkAddress &address);

public:

	KDp2p_ConnectionNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos);

	virtual ~KDp2p_ConnectionNode();

	/**
		Returns the maximum number of connections this node will keep track of.
		level 0 = root node
	*/
	int GetMaxNumberOfConnections(int level);

	/**
		Returns the type of the node: ConnectionNode
	*/
	virtual KDp2p_NodeType GetNodeType();

	/**
		Picks an IP at random amongst the connection node.
	*/
	virtual KDp2p_NetworkAddress PickOneRandomAddress();

	/**
		Browses the tree and puts in the 'addresses' set all the addresses of the ConnectionNodes of the tree.
	*/
	virtual void BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses);

	/**
		Tries to reach the given node.
		If the node is not part of the tree, returns the "closest" ConnectionNode available or a leaf node if a leaf node has been reached.
	*/
	virtual KDp2p_BNode *FindClosestNode(const KDp2p_BPosition &askedPos);

	/**
		Adds the given IP address to the ConnectionNode.
		The address will be added in first position.
		First, a check will be done so that the address is not already present.
		If it is, the method will return false.
		Otherwise, true.
	*/
	bool AddNetworkAddress(const KDp2p_NetworkAddress &address);

	/**
		Puts all the connections in the deque.
		This method is nearly similar to BrowseTreeForIPAddresses except that
		GetAllAddresses puts the result in a deque (which keeps the order of insertion)
	*/
	void GetAllAddresses(deque<KDp2p_NetworkAddress> &addresses);

	/**
		Inherited from KDp2p_ConnectionListener
		Sent when a connection cannot be established.
	*/
	virtual void ConnectionFailed(KDp2p_ConnectionHandler *address);

	virtual void ConnectionInterrupted(KDp2p_ConnectionHandler *address);

	virtual void ConnectionSucceeded(KDp2p_ConnectionHandler *address);

	virtual void ConnectionClosedByPeer(KDp2p_ConnectionHandler *address);

	/**
		Adds the given IP to the black-list.
		If it is part of the connections followed by this node, it is removed.
		The IP will remain on the black-list engine->connectionTimeOut*1.5 seconds.
		The mehod is executing the ClearBlackList method.
	*/
	void AddToBlackList(const KDp2p_NetworkAddress &address);

	/**
		Returns true if a connection is black-listed
	*/
	bool IsBlackListed(const KDp2p_NetworkAddress &address);
};

#endif
