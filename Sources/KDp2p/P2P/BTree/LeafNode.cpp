#include "LeafNode.h"

KDp2p_LeafNode::KDp2p_LeafNode(KDp2p_BTree *_tree) :  KDp2p_BNode(_tree)
{

}

KDp2p_LeafNode::~KDp2p_LeafNode()
{

}

KDp2p_NodeType KDp2p_LeafNode::GetNodeType()
{
	return LeafNodeType;
}

void KDp2p_LeafNode::BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses)
{
	return;
}

KDp2p_BNode *KDp2p_LeafNode::FindClosestNode(const KDp2p_BPosition &pos, int level)
{
	return this;
}
