#include "BTree.h"

#include "BNode.h"
#include "BranchNode.h"
#include "ConnectionNode.h"
#include "LeafNode.h"

KDp2p_BTree::KDp2p_BTree(KDp2p_P2PEngine *_engine, int _maxDepth) : engine(_engine), maxDepth(_maxDepth), root(0), nbIPsForConnectionNodes(NBIPSFORCONNECTIONNODES)
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
	if (this->root)
		delete this->root;

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
	return root->FindClosestNode(pos);
}

bool KDp2p_BTree::AddLeaf(const KDp2p_BPosition &pos, KDp2p_SerializableObject *object)
{
	KDp2p_BNode *currentNode = root;

	//do
	//{
	currentNode = FindClosestNode(pos);

	// Did we find the leaf (we are not supposed to)
	if (currentNode->GetPosition().GetNBBits() == pos.GetNBBits())
	{
		// return false? Do we really want to do that???
		// Shall I throw an exception instead?
		return false;
	}

	// Ok, now let's ask for help from our friends the peers.
	// From that point, we ask a question, when we receive the answer, we will ask another question
	// and so on until we reach the node
	

	//} while (1);

	return true;
}

char KDp2p_BTree::GetNbIPsForConnectionNodes()
{
	return nbIPsForConnectionNodes;
}

bool KDp2p_BTree::AddLeafDirectly(const KDp2p_BPosition &leafPos, KDp2p_SerializableObject *object)
{
	KDp2p_BNode *startingNode = FindClosestNode(leafPos);

	if (startingNode->GetNodeType() != BranchNodeType)
		return false;

	return AddLeafDirectly((KDp2p_BranchNode*)startingNode, leafPos, object);		
}

bool KDp2p_BTree::AddLeafDirectly(KDp2p_BranchNode *startingNode, const KDp2p_BPosition &leafPos, KDp2p_SerializableObject *object)
{
	KDp2p_BranchNode *currentNode = startingNode;

	if (currentNode == 0)
	{
		if (root==0)
		{
			root = new KDp2p_BranchNode(this, KDp2p_BPosition());
		}
		else if (root->GetNodeType() != BranchNodeType)
			return false;

		currentNode = (KDp2p_BranchNode *) root;	
	}

	while (leafPos.GetNBBits()-1 > currentNode->GetPosition().GetNBBits())
	{
		// first, let's find the child
		bool direction = leafPos.GetBit(currentNode->GetPosition().GetNBBits());
		// ok, the child in the direction is a BranchNode
		// the child in the opposite direction is an empty connectionnode.
		KDp2p_BPosition posDirection = currentNode->GetPosition();
		posDirection.AddBit(direction);
		KDp2p_BPosition posOppositeDirection = currentNode->GetPosition();
		posOppositeDirection.AddBit(!direction);

		KDp2p_BranchNode *nextNode = new KDp2p_BranchNode(this, posDirection);
		currentNode->SetChild(direction, nextNode);

		// There could be an existing ConnectionNode at the first iteration. In this case, we don't want to create the empty ConnectionNode.
		// On the other iterations, there won't be any existing ConnectionNodes
		if (currentNode->GetChild(!direction) == 0)
			currentNode->SetChild(!direction, new KDp2p_ConnectionNode(this, posOppositeDirection));

		currentNode = nextNode;
	}

	// currentNode now points to the last BranchNode before the LeafNode
	bool direction = leafPos.GetBit(currentNode->GetPosition().GetNBBits());
	// ok, the child in the direction is the leafNode
	// the child in the opposite direction is an empty connectionNode.
	KDp2p_BPosition posDirection = currentNode->GetPosition();
	posDirection.AddBit(direction);
	KDp2p_BPosition posOppositeDirection = currentNode->GetPosition();
	posOppositeDirection.AddBit(!direction);

	currentNode->SetChild(direction, new KDp2p_LeafNode(this, posDirection));
	currentNode->SetChild(!direction, new KDp2p_ConnectionNode(this, posOppositeDirection));

	return true;
}
