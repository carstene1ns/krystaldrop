#ifndef BTree_H
#define BTree_H

#include "../../Tools/defines.h"

#include "BPosition.h"

class KDp2p_BNode;
class KDp2p_BranchNode;
class KDp2p_P2PEngine;
class KDp2p_SerializableObject;

/**
	A Distributed Binary Tree
*/
class DllExport KDp2p_BTree
{
protected:
	/// Root node of the tree
	KDp2p_BNode *root;

	/// Maximum depth of the tree
	int maxDepth;

	/// P2PEngine pointer
	KDp2p_P2PEngine *engine;

#define NBIPSFORCONNECTIONNODES 3
	/**
		The number of peers each connectionNode should keep track of.
		By default equals to NBIPSFORCONNECTIONNODES
	*/
	char nbIPsForConnectionNodes;

public:

	KDp2p_BTree(KDp2p_P2PEngine *_engine, int _maxDepth);

	virtual ~KDp2p_BTree();

	/// Sets the maximum depth of the binary tree
	void SetMaxDepth(int maxDepth);

	/// Returns the maximum depth of the binary tree
	int GetMaxDepth();

	/// Sets the root node
	void SetRootNode(KDp2p_BNode *root);

	/// Returns the root node of the binary tree
	KDp2p_BNode *GetRootNode();

	/// Returns a pointer to the P2P engine
	KDp2p_P2PEngine *GetEngine();

	/**
		Tries to reach the given node.
		If the node is not part of the tree, returns the "closest" ConnectionNode available or a leaf node if a leaf node has been reached.
	*/
	KDp2p_BNode *FindClosestNode(const KDp2p_BPosition &pos);

	/**
		Adds a brand new leaf to the tree
		The object "object" is added to a leaf node placed at position "pos"
		This method will use the neighbour peers and build the needed parts of the tree to add the object.
		This method can be directly used by the "end-user"
	*/
	bool AddLeaf(const KDp2p_BPosition &pos, KDp2p_SerializableObject *object);

	/**
		Returns the number of peers each connectionNode should keep track of.
	*/
	char GetNbIPsForConnectionNodes();

	//{
	/**
		This method adds a subtree from the startingNode.
		This subtree will go down to the leafNode.
		In between, there will be BranchNodes and empty ConnectionNodes.
		The node startingNode, must exist and must be a BranchNode.
		If 0 is passed, the whole tree is built from the root node.
		This method DOES NOT use the neighbour peers.
		This method should not be used by the "end-user".
	*/
	bool AddLeafDirectly(const KDp2p_BPosition &leafPos, KDp2p_SerializableObject *object);
	bool AddLeafDirectly(KDp2p_BranchNode *startingNode, const KDp2p_BPosition &leafPos, KDp2p_SerializableObject *object);
	//}
};

#endif
