#include "LeafNode.h"

#include "SerializableObject.h"

KDp2p_LeafNode::KDp2p_LeafNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos) :  KDp2p_BNode(_tree, _pos), object(0)
{

}

KDp2p_LeafNode::~KDp2p_LeafNode()
{
	if (object)
		delete object;
}

KDp2p_NodeType KDp2p_LeafNode::GetNodeType()
{
	return LeafNodeType;
}

void KDp2p_LeafNode::BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses)
{
	return;
}

KDp2p_BNode *KDp2p_LeafNode::FindClosestNode(const KDp2p_BPosition &askedPos)
{
	return this;
}
