#include "BNode.h"

KDp2p_BNode::KDp2p_BNode()
{
	children[0] = 0;
	children[1] = 0;
}

KDp2p_BNode::~KDp2p_BNode()
{
	if (children[0])
		delete children[0];
	if (children[1])
		delete children[1];
}

KDp2p_BNode *KDp2p_BNode::GetChild(bool child) const
{
	return children[(child)?1:0];
}

void KDp2p_BNode::SetChild(bool child, KDp2p_BNode *node)
{
	if (children[(child)?1:0] != 0)
		delete children[(child)?1:0];

	children[(child)?1:0] = node;
}
