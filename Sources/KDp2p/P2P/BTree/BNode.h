#ifndef BNode_H
#define BNode_H

#include "../../Tools/defines.h"
#include "BPosition.h"
#include "../../Network/NetworkAddress.h"

class KDp2p_BTree;

#include <set>
#include <vector>
using namespace std;

#include "BPosition.h"

class KDp2p_BranchNode;

enum KDp2p_NodeType
{
	BranchNodeType = 0,
	ConnectionNodeType,
	LeafNodeType
};

/**
	The base class for binary nodes.
*/
class DllExport KDp2p_BNode
{
protected:
	/// The BTree
	KDp2p_BTree *tree;

	/// The position in the tree
	KDp2p_BPosition pos;

public:

	/**
		Constructor must be given the BTree containing the Node and the position of the node.
	*/
	KDp2p_BNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos);

	virtual ~KDp2p_BNode();

	/**
		Pure virtual method.
		Returns the type of the node.
	*/
	virtual KDp2p_NodeType GetNodeType()=0;

	/**
		Pure virtual function
		Browses the tree and puts in the 'addresses' vector all the addresses of the ConnectionNodes of the tree.
	*/
	virtual void BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses)=0;

	/**
		Picks "number" IPs at random amongst the connection nodes of the subtree and puts them in the "addresses" vector.
        The random algorithm will select the IP uniformely over the known IPs (so not uniformely amongst the space of keys)
	*/
	void PickRandomAddress(vector<KDp2p_NetworkAddress> &addresses, int number);

	/**
		Tries to reach the given node.
		If the node is not part of the tree, returns the "closest" ConnectionNode available or a leaf node if a leaf node has been reached.
	*/
	virtual KDp2p_BNode *FindClosestNode(const KDp2p_BPosition &askedPos)=0;

	/**
		Returns the position of the node in the tree
	*/
	const KDp2p_BPosition &GetPosition();

	/**
		Returns the parentNode (method quite slow due to implementation)
		Returns 0 if the parent is the rootNode
	*/
	KDp2p_BranchNode *GetParent();
};

#endif
