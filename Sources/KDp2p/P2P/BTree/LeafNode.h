#ifndef LeafNode_H
#define LeafNode_H

#include "../../Tools/defines.h"

#include "BNode.h"

class KDp2p_SerializableObject;

/**
	A final leaf in the Distributed P2P tree, used to store the accessed object.
*/
class DllExport KDp2p_LeafNode : public KDp2p_BNode
{
	KDp2p_SerializableObject *object;

public:

	KDp2p_LeafNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos);

	/**
		When destroyed, destroy the stored object.
	*/
	virtual ~KDp2p_LeafNode();

	/**
		Returns the type of the node: ConnectionNode
	*/
	virtual KDp2p_NodeType GetNodeType();

	/**
		Browses the tree and puts in the 'addresses' vector all the addresses of the ConnectionNodes of the tree.
	*/
	virtual void BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses);

	/**
		Tries to reach the given node.
		If the node is not part of the tree, returns the "closest" ConnectionNode available or a leaf node if a leaf node has been reached.
	*/
	virtual KDp2p_BNode *FindClosestNode(const KDp2p_BPosition &askedPos);
};

#endif
