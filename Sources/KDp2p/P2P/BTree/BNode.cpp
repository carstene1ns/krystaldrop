#include "BNode.h"

#include "BTree.h"

KDp2p_BNode::KDp2p_BNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos) : tree(_tree), pos(_pos)
{
}

KDp2p_BNode::~KDp2p_BNode()
{
}

void KDp2p_BNode::PickRandomAddress(vector<KDp2p_NetworkAddress> &addresses, int number)
{
	if (number <= 0)
		return;

	set<KDp2p_NetworkAddress> setAddresses;
	
	// Fill in all the addresses from the subtree
	BrowseTreeForIPAddresses(setAddresses);

	// Now pick "number" addresses amongst the addresses returned
	for (int i=0; i<number; i++)
	{
		int nbAddresses = (int)setAddresses.size();

		// If there are no more addresses in the set, return.
		if (nbAddresses == 0)
			break;

		int no = (int)((((double)rand())/RAND_MAX)*nbAddresses);

		set<KDp2p_NetworkAddress>::iterator it = setAddresses.begin();

		for (int j=0; j<no; j++)
			it++;
		
		addresses.push_back(*it);

		setAddresses.erase(it);
	}
}

const KDp2p_BPosition &KDp2p_BNode::GetPosition()
{
	return pos;
}

KDp2p_BranchNode *KDp2p_BNode::GetParent()
{
	if (pos.GetNBBits() == 0)
		return 0;

	KDp2p_BPosition posParent = pos;
	posParent.RemoveBit();

#ifdef DEBUG
	if (tree->FindClosestNode(posParent)->GetNodeType() !=  BranchNodeType)
	{
		printf("WARNING! Unable to retrieve parent node!!!! Totally anormal!!!");
		return 0;
	}
#endif

	return (KDp2p_BranchNode *) tree->FindClosestNode(posParent);
}
