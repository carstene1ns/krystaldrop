#include "BranchNode.h"

#include "ConnectionNode.h"

KDp2p_BranchNode::KDp2p_BranchNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos) : KDp2p_BNode(_tree, _pos)
{
	children[0] = 0;
	children[1] = 0;
}

KDp2p_BranchNode::~KDp2p_BranchNode()
{
	if (children[0])
		delete children[0];
	if (children[1])
		delete children[1];
}

KDp2p_BNode *KDp2p_BranchNode::GetChild(bool child) const
{
	return children[(child)?1:0];
}

void KDp2p_BranchNode::SetChild(bool child, KDp2p_BNode *node)
{
	if (children[(child)?1:0] != 0)
		delete children[(child)?1:0];

	children[(child)?1:0] = node;
}

KDp2p_NodeType KDp2p_BranchNode::GetNodeType()
{
	return BranchNodeType;
}

/*KDp2p_NetworkAddress KDp2p_BranchNode::PickRandomAddress()
{
	int randomNumber = rand();

	int direction;

	if (randomNumber < RAND_MAX/2)
		direction = 0;
	else
		direction = 1;

	// positif ou pas le rand?

	switch (children[direction]->GetNodeType())
	{
	case BranchNodeType:
		return ((KDp2p_BranchNode*)children[direction])->PickRandomAddress();
		break;
	case ConnectionNodeType:
		return ((KDp2p_ConnectionNode*)children[direction])->PickRandomAddress();
		break;
	default:
	case LeafNodeType:
		// A TERMINER
		// PB: ET SI ON VEUT PLUSIEURS NOMBRE TIRES AU HASARD???
		break;
	}

}*/

void KDp2p_BranchNode::BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses)
{
	if (children[0]!=0)
		children[0]->BrowseTreeForIPAddresses(addresses);
	if (children[1]!=0)
		children[1]->BrowseTreeForIPAddresses(addresses);
	return;
}

KDp2p_BNode *KDp2p_BranchNode::FindClosestNode(const KDp2p_BPosition &askedPos)
{
	// if level == level of the BPosition, we're done!
	if (pos.GetNBBits() == askedPos.GetNBBits())
		return this;

	bool b = askedPos.GetBit(pos.GetNBBits());

	return children[(b)?1:0]->FindClosestNode(askedPos);
}
