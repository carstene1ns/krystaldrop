#include "ConnectionNode.h"

#include "BTree.h"
#include "../ConnectionManager.h"
#include "../P2PEngine.h"

KDp2p_ConnectionNode::KDp2p_ConnectionNode(KDp2p_BTree *_tree) : KDp2p_BNode(_tree)
{

}

KDp2p_ConnectionNode::~KDp2p_ConnectionNode()
{
	deque<KDp2p_NetworkAddress>::iterator it = connections.begin();
	
	while (it != connections.end())
	{
		tree->GetEngine()->GetConnectionManager()->CloseConnection(&(*it));
		it++;
	}
}

int KDp2p_ConnectionNode::GetMaxNumberOfConnections(int level)
{
	return (1<<(tree->GetMaxDepth()-level-1))-1;
}

KDp2p_NodeType KDp2p_ConnectionNode::GetNodeType()
{
	return ConnectionNodeType;
}

KDp2p_NetworkAddress KDp2p_ConnectionNode::PickRandomAddress()
{
	int no = (int)((((double)rand())/RAND_MAX)*connections.size());

	return connections[no];
}

void KDp2p_ConnectionNode::BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses)
{
	deque<KDp2p_NetworkAddress>::iterator it = connections.begin();

	while (it != connections.end())
	{
		addresses.insert((*it));
	}
}

KDp2p_BNode *KDp2p_ConnectionNode::FindClosestNode(const KDp2p_BPosition &pos, int level)
{
	return this;
}
