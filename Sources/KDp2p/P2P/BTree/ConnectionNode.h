#ifndef ConnectionNode_H
#define ConnectionNode_H

#include "../../Tools/defines.h"

#include "BNode.h"
#include "../ConnectionListener.h"

#include <deque>
using namespace std;

/**
	A leaf in the Distributed P2P tree, used to gain access to the rest of the tree from the P2P network.
*/
class DllExport KDp2p_ConnectionNode : public KDp2p_BNode, KDp2p_ConnectionListener
{
	/**
		The IP Adresses in this deque are possibly usable to access the rest of the tree.
	*/
	deque<KDp2p_NetworkAddress> connections;

public:

	KDp2p_ConnectionNode(KDp2p_BTree *_tree);

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
	virtual KDp2p_NetworkAddress PickRandomAddress();

	/**
		Browses the tree and puts in the 'addresses' vector all the addresses of the ConnectionNodes of the tree.
	*/
	virtual void BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses);

	/**
		Tries to reach the given node.
		If the node is not part of the tree, returns the "closest" ConnectionNode available or a leaf node if a leaf node has been reached.
	*/
	virtual KDp2p_BNode *FindClosestNode(const KDp2p_BPosition &pos, int level);
};

#endif
