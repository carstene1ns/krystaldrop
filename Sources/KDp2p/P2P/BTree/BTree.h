#ifndef BTree_H
#define BTree_H

#include "../../Tools/defines.h"

class KDp2p_BNode;
class KDp2p_P2PEngine;

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

	*/
	bool AddLeaf(const KDp2p_BPosition &pos, KDp2p_SerializableObject *object);
};

#endif
