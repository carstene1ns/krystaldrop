#include "BTree.h"

#include "BNode.h"

KDp2p_BTree::KDp2p_BTree(KDp2p_P2PEngine *_engine, int _maxDepth) : engine(_engine), maxDepth(_maxDepth), root(0)
{
	
}

KDp2p_BTree::~KDp2p_BTree()
{
	if (root)
		delete root;
}

void KDp2p_BTree::SetMaxDepth(int maxDepth)
{
	this->maxDepth = maxDepth;
}

int KDp2p_BTree::GetMaxDepth()
{
	return maxDepth;
}

void KDp2p_BTree::SetRootNode(KDp2p_BNode *root)
{
	if (root)
		delete root;

	this->root = root;
}

KDp2p_BNode *KDp2p_BTree::GetRootNode()
{
	return root;
}

KDp2p_P2PEngine *KDp2p_BTree::GetEngine()
{
	return engine;
}

KDp2p_BNode *KDp2p_BTree::FindClosestNode(const KDp2p_BPosition &pos)
{
	return root->FindClosestNode(pos, 0);
}

bool KDp2p_BTree::AddLeaf(const KDp2p_BPosition &pos, KDp2p_SerializableObject *object)
{
	KDp2p_BNode *currentNode = root;

	do
	{
		currentNode = FindClosestNode(pos);



	} while (1);
}
