#ifndef BranchNode_H
#define BranchNode_H

#include "../../Tools/defines.h"

#include "BNode.h"
#include "../../Network/NetworkAddress.h"

class KDp2p_SerializableObject;

/**
	A branch node
*/
class DllExport KDp2p_BranchNode : public KDp2p_BNode
{
	/// Children nodes
	(KDp2p_BNode*) children[2];

public:

	KDp2p_BranchNode(KDp2p_BTree *_tree);

	virtual ~KDp2p_BranchNode();

	/**
		Returns the specified child
	*/
	KDp2p_BNode *GetChild(bool child) const;

	/**
		Sets the specified child, if the node was previously allocated, it is deleted.
	*/
	void SetChild(bool child, KDp2p_BNode *node);

	/**
		Returns the type of the node: BranchNodeType
	*/
	virtual KDp2p_NodeType GetNodeType();

	/**
		Picks an IP at random amongst the connection nodes of the subtree.
        The random algorithm will select the IP uniformely over the space of keys (so not uniformely amongst the known IPs)
	*/
//	virtual KDp2p_NetworkAddress PickRandomAddress();

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
