#include "ConnectionNode.h"

#include "BTree.h"
#include "../ConnectionManager.h"
#include "../P2PEngine.h"
#include "../ConnectionHandler.h"

KDp2p_ConnectionNode::KDp2p_ConnectionNode(KDp2p_BTree *_tree, const KDp2p_BPosition &_pos) : KDp2p_BNode(_tree, _pos)
{

}

KDp2p_ConnectionNode::~KDp2p_ConnectionNode()
{
	//deque<KDp2p_NetworkAddress>::iterator it = connections.begin();
	deque<KDp2p_ConnectionHandler*>::iterator it = connections.begin();
	
	printf("Deleting ConnectionNode.\n");

	while (it != connections.end())
	{
		tree->GetEngine()->GetConnectionManager()->CloseConnection(*it);
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

KDp2p_NetworkAddress KDp2p_ConnectionNode::PickOneRandomAddress()
{
	int no = (int)((((double)rand())/RAND_MAX)*connections.size());

	return connections[no]->address;
}

void KDp2p_ConnectionNode::BrowseTreeForIPAddresses(set<KDp2p_NetworkAddress> &addresses)
{
	//deque<KDp2p_NetworkAddress>::iterator it = connections.begin();
	deque<KDp2p_ConnectionHandler*>::iterator it = connections.begin();

	while (it != connections.end())
	{
		addresses.insert((*it)->address);
		it++;
	}
}

KDp2p_BNode *KDp2p_ConnectionNode::FindClosestNode(const KDp2p_BPosition &askedPos)
{
	return this;
}

bool KDp2p_ConnectionNode::AddNetworkAddress(const KDp2p_NetworkAddress &address)
{
	// First, let's check whether the IP address is already part of the connectionNode.
	// It shouldn't!
	deque<KDp2p_ConnectionHandler *>::iterator it = connections.begin();

	while (it != connections.end())
	{
		if ((*it)->address == address)
			return false;
		it++;
	}

	// Second lt's check if it is part of the black-list
	// It shouldn't either!
	deque<BannedAddress>::iterator it2 = blackList.begin();

	while (it2 != blackList.end())
	{
		if ((*it2).address == address)
			return false;
		it2++;
	}

	connections.push_front(tree->GetEngine()->GetConnectionManager()->AddConnection(address, this));
	return true;
}

void KDp2p_ConnectionNode::GetAllAddresses(deque<KDp2p_NetworkAddress> &addresses)
{
	deque<KDp2p_ConnectionHandler*>::iterator it = connections.begin();

	while (it != connections.end())
	{
		addresses.push_back((*it)->address);
		it++;
	}
}

void KDp2p_ConnectionNode::ConnectionFailed(KDp2p_ConnectionHandler *address)
{
	// Ok, we could not establish the connection.
	// We should remove the connection from the ConnectionNode
	// Then we should ask for more IPs for that node.
	// Problem: we don't want the IP we had trouble with!
	// so maybe we should send a message to the node we will inquire saying:
	// "ok lad, give me some peers for node 'pos' without the IP ip"
	// But what if 2 nodes fail together?
	// Maybe we should suscribe the IP failing to a "black-list".
	// The black-list should contain IPs stored with there date.
	// When the date is some time old, we remove the IP from the black-list.
	AddToBlackList(address->address);
}

void KDp2p_ConnectionNode::ConnectionInterrupted(KDp2p_ConnectionHandler *address)
{
	RemoveConnection(address->address);
}

void KDp2p_ConnectionNode::ConnectionSucceeded(KDp2p_ConnectionHandler *address)
{

}

void KDp2p_ConnectionNode::ConnectionClosedByPeer(KDp2p_ConnectionHandler *address)
{
	RemoveConnection(address->address);
}

void KDp2p_ConnectionNode::ClearBlackList()
{
	unsigned int time = SDL_GetTicks();

	while (!blackList.empty() && blackList[0].time < time)
	{
		blackList.pop_front();
	}
}

void KDp2p_ConnectionNode::RemoveConnection(const KDp2p_NetworkAddress &address)
{
	deque<KDp2p_ConnectionHandler *>::iterator it = connections.begin();

	while (it != connections.end())
	{
		if (address == (*it)->address)
		{
			connections.erase(it);
			break;
		}
		it++;
	}
}

void KDp2p_ConnectionNode::AddToBlackList(const KDp2p_NetworkAddress &address)
{
	ClearBlackList();

	deque<KDp2p_ConnectionHandler *>::iterator it = connections.begin();
	printf("Adding peer %s to one of our connection nodes.\n",address.ToString().c_str());

	while (it != connections.end())
	{
		if (address == (*it)->address)
		{
			// a connection black-listed can still be valid if the connection still exists
			// but the peer does not have the information requested for this node.
			// so it makes sense to close it.
			tree->GetEngine()->GetConnectionManager()->CloseConnection(*it);
			connections.erase(it);
			break;
		}
		it++;
	}

	blackList.push_back(BannedAddress(address, SDL_GetTicks() + (unsigned int)(((float)tree->GetEngine()->GetConnectionTimeOut())*1.5f)));
}

bool KDp2p_ConnectionNode::IsBlackListed(const KDp2p_NetworkAddress &address)
{
	ClearBlackList();

	deque<BannedAddress>::iterator it = blackList.begin();

	while (it != blackList.end())
	{
		if ((*it).address == address)
			return true;
		it++;
	}
	return false;
}
