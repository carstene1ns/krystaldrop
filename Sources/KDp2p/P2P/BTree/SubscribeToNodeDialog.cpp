#include "SubscribeToNodeDialog.h"

#include "../../Network/Message.h"
#include "../../Network/NetworkAddress.h"
#include "BPosition.h"
#include "../../../KDpp/Tools/LogFile.h"
#include "BTree.h"
#include "BNode.h"
#include "BranchNode.h"
#include "ConnectionNode.h"
#include "../P2PEngine.h"

KDp2p_SubscribeToNodeDialog::KDp2p_SubscribeToNodeDialog(KDp2p_P2PEngine *engine) : KDp2p_Dialog(engine, "STNO")
{
	
}

KDp2p_SubscribeToNodeDialog::KDp2p_SubscribeToNodeDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message, int questionType) : KDp2p_Dialog(engine, message,questionType)
{

}

KDp2p_SubscribeToNodeDialog::~KDp2p_SubscribeToNodeDialog()
{

}

void KDp2p_SubscribeToNodeDialog::OnTimeOut()
{
	// Ok, we did a timeout.
	// Have we got alternativeNodes to try?
	AskAlternativePeer();
}

void KDp2p_SubscribeToNodeDialog::OnAnswer()
{
	// Ok, first, let's read the code
	// It can be E (requested a node not available), U (goal node already used)
	// or V (valid answer)

	char code = answer->GetByte();

	switch (code)
	{
	case 'E':
		{
		printf("We asked peer %s a node with SubscribeToNode. This node is not available in its DBT.");
		
		AskAlternativePeer();
		}
	case 'U':
		// Ooooops!
		// The node we want is already used!
		// we should have a call-back here at least to tell the caller something went wrong!
		printf("Problem: we asked to suscribe to a node already used!\nPeer %s is using it!\n",answer->GetAddress()->ToString().c_str());
		// TO DO!!!!!
		return;
	}
	
	// case V:
	
	// Let's find the parent of the node we want to split.
	KDp2p_BTree *bTree = (KDp2p_BTree *) GetEngine()->GetUniqueObject(treeID);
	KDp2p_BranchNode *parentNode;
	// Whatever the node we wanted details from, we might as well already know it
	// if we asked details to someone else for another reason, whatever.
	// So let's have a look at what node we need.
	KDp2p_BPosition newNodePos = KDp2p_BPosition();
	if (bTree->GetRootNode() != 0)
		newNodePos = bTree->FindClosestNode(goalPos)->GetPosition();
	
	if (newNodePos == goalPos)
	{
		// Ok, while we were searching for goalPos, we've had it another way.
		// So there is no point in continuing....
		// strange, but why not.
		return;
	}
	
	if (newNodePos.GetNBBits() > nodePos.GetNBBits())
	{
		// if we gained the knowledge of a part of the subtree while the call was done.
		// so a part of the answer is redondant and must be discarded.
		KDp2p_NetworkAddress nullAddress;
				
		for (int i=0; i<newNodePos.GetNBBits() - nodePos.GetNBBits(); i++)
		{
			char nbConnections = answer->GetByte();

			for (int j=0; j<nbConnections; j++)
			{
				nullAddress.ReadFromMessage(answer);
			}
		}
	}
	else if (newNodePos.GetNBBits() < nodePos.GetNBBits())
	{
		// if we lost the knowledge of a part of the tree while the call was done
		// we are in trouble. we know the bottom of the tree, but that's useless
		// since we don't know the upper part.
		// Let's ask the question again... from upper
		printf("KDp2p_SubscribeToNodeDialog::OnAnswer() : We lost the knowledge of a part of the tree while the call was done. We know the bottom of the tree, but that's useless since we don't know the upper part. Let's ask the question again... from upper.");

		KDp2p_SubscribeToNodeDialog *newDialog = new KDp2p_SubscribeToNodeDialog(GetEngine());
		newDialog->SetNodeToRetrieve(treeID, newNodePos, goalPos, object);
		// Luckily, we can ask the question to the same computer
		newDialog->GetQuestion()->SetAddress(*(answer->GetAddress()));
		
		deque<KDp2p_NetworkAddress>::iterator itAlternativePeers = alternativePeers.begin();
		while (itAlternativePeers != alternativePeers.end())
		{
			newDialog->AddAlternativePeer(*itAlternativePeers);
			itAlternativePeers++;
		}
		newDialog->SendQuestion();		

		return;
	}
	

	// if we requested the root node, let's do the first step a different way.
	if (newNodePos.GetNBBits() == 0)
	{
		// FIRST, What should we do with the root node
		// It is a ConnectionNode!
		// We should delete it and replace it with a BranchNode
		parentNode = new KDp2p_BranchNode(bTree, KDp2p_BPosition());
		bTree->SetRootNode(parentNode);

		// ok, let's fill in the ConnectionNode child!
		// which is the ConnectionNode?
		bool direction = goalPos.GetBit(0);
		KDp2p_BPosition posConnectionNode = newNodePos;
		posConnectionNode.AddBit(!direction);

		KDp2p_ConnectionNode *connectionNode = new KDp2p_ConnectionNode(bTree, posConnectionNode);

		parentNode->SetChild(!direction, connectionNode);

		char nbConnections = answer->GetByte();
		KDp2p_NetworkAddress peerAddress;

		for (int i=0; i<nbConnections; i++)
		{
			peerAddress.ReadFromMessage(answer);	
			connectionNode->AddNetworkAddress(peerAddress);
		}

	}
	else
	{	
		KDp2p_BPosition parentPos = nodePos;
		parentPos.RemoveBit();
		
		parentNode = (KDp2p_BranchNode*) bTree->FindClosestNode(parentPos);
	}


	char nbConnections = answer->GetByte();
	KD_LogFile::printf2("Lit un char: %d\n", nbConnections);
	bool direction = goalPos.GetBit(parentNode->GetPosition().GetNBBits());
	
	while (nbConnections != -1)
	{
		KDp2p_BPosition posNewParentNode = parentNode->GetPosition();
		posNewParentNode.AddBit(direction);

		KDp2p_BranchNode *newParentNode = new KDp2p_BranchNode(bTree, posNewParentNode);
		parentNode->SetChild(direction, newParentNode);
		parentNode = newParentNode;

		direction = goalPos.GetBit(parentNode->GetPosition().GetNBBits());

		KDp2p_BPosition posConnectionNode = parentNode->GetPosition();
		posConnectionNode.AddBit(!direction);
		KDp2p_ConnectionNode *connectionNode = new KDp2p_ConnectionNode(bTree, posConnectionNode);
		parentNode->SetChild(!direction, connectionNode);

#ifdef DEBUG
		if (!goalPos.IsAChildOf(parentNode->GetPosition()))
		{
			printf("WARNING! we have a buggy set of nodePos / goalPos!\nStupid us!!!!\n");
			printf("goalPos %s\n", goalPos.ToString().c_str());
			printf("nodePos %s\n", parentNode->GetPosition().ToString().c_str());
		}
#endif

		KDp2p_NetworkAddress peerAddress;

		for (int i=0; i<nbConnections; i++)
		{
			peerAddress.ReadFromMessage(answer);	
			KD_LogFile::printf2("Lit une addresse: %s\n", peerAddress.ToString().c_str());
			connectionNode->AddNetworkAddress(peerAddress);
		}

		nbConnections = answer->GetByte();
		KD_LogFile::printf2("Lit un char: %d\n", nbConnections);
	};

	// Ok, every node has been created except the last one.
	// Now, we want to ask the same question to the peers passed at the end of the message
	// since they now more about the subtree.
	// If none has been passed, we are done! We just need to add the object to the end of the tree.
	nbConnections = answer->GetByte();
	KD_LogFile::printf2("Lit un char: %d\n", nbConnections);

	if (nbConnections == 0)
	{
		bTree->AddLeafDirectly(parentNode, goalPos, object);
		return;		// YES!
	}
	
	// We have to ask more questions to other peers...
	// Ok, here is the question:

	KDp2p_SubscribeToNodeDialog *newDialog = new KDp2p_SubscribeToNodeDialog(GetEngine());

	KDp2p_BPosition posNextNodeToAsk = parentNode->GetPosition();
	direction = goalPos.GetBit(parentNode->GetPosition().GetNBBits());
	posNextNodeToAsk.AddBit(direction);

	// let's add a ConnectionNode instead of the node we are looking for.
	KDp2p_ConnectionNode *connectionNode = new KDp2p_ConnectionNode(bTree, posNextNodeToAsk);
	parentNode->SetChild(direction, connectionNode);
		
	newDialog->SetNodeToRetrieve(treeID, posNextNodeToAsk, goalPos, object);

	KDp2p_NetworkAddress peerAddress;
	peerAddress.ReadFromMessage(answer);
	KD_LogFile::printf2("Lit l'addresse du node auquel demander suivant: %s\n", peerAddress.ToString().c_str());
	newDialog->GetQuestion()->SetAddress(peerAddress);

	for (int i=1; i<nbConnections; i++)
	{
		peerAddress.ReadFromMessage(answer);
		KD_LogFile::printf2("Lit les addresses alternatives en cas d'echec: %s\n", peerAddress.ToString().c_str());
		newDialog->AddAlternativePeer(peerAddress);
	}

	// YEAHAAA!
	newDialog->SendQuestion();
}

void KDp2p_SubscribeToNodeDialog::OnQuestion(KDp2p_DialogFactory *factory)
{
	// The nodes the requester wants to subscribe to
	treeID = question->GetInt();
	nodePos.ReadFromMessage(question);
	goalPos.ReadFromMessage(question);

	// here, we check that goalPos is a chidren of nodePos
	if (!goalPos.IsAChildOf(nodePos))
	{
		printf("WARNING! In KDp2p_SubscribeToNodeDialog::OnQuestion, the requester sent us a buggy set of nodePos / goalPos!\nThis should never happen with standard clients. Not answering anything.\n");
		return;
	}
	
	// Now we must send our knowledge of the part of the tree we know
	// Ok, let's find the nodePos node.
	KDp2p_BTree *bTree = (KDp2p_BTree *) GetEngine()->GetUniqueObject(treeID);

	if (bTree->GetRootNode() == 0)
	{
		// We should raise an exception here!
		printf("Error! We are asked a question about the tree but we don't have any knowledge about it!\nThe most likely reason is that we are asking details to ourselves at the very beginning of the building of the tree.\nThe peerList must contain our IP address.");
	}

	KDp2p_BNode *currentNode = bTree->FindClosestNode(nodePos);

	// If we could not retrieve the asked node, there has been a mistake.
	// The asker should not have asked for this node.
	if (currentNode->GetPosition() != nodePos)
	{
		// Let's answer him there is a mistake.
		// Code: 'E'
		printf("Peer %s asked a node with SubscribeToNode. This node is not available in our DBT.");
		answer->AddChar('E');
		// On reception, the receiver should remove us from the ConnectionNode
		SendAnswer();
		return;
	}

	// If we have the goalNode, the asker can't get it for its tree!
	if (bTree->FindClosestNode(goalPos)->GetPosition() == goalPos)
	{
		// Send him an error message
		// Code 'U' (Used)
		answer->AddChar('U');
		// On reception, the receiver should try another key
		SendAnswer();
		return;
	}

	// Ok, we have the node Pos and we don't have goalPos, let's make the answer
	// Valid answer: Code 'V'
	answer->AddChar('V');
	
	// ok, currentNode has 2 children, we want to send details from the one
	// the opposite to goalPos
	int currentPosDepth;

	do
	{
		// Let's take the opposite direction.
		// currentNode should be a BranchNode. If not, there is a problem!
		// Problem: error code N is difficult to implement because we can't
		// go back in a message already written.
		/*if (currentNode->GetNodeType() != BranchNodeType)
		{
			// ok, problem.
			// let's send an error message to our requester...
			// Error Code 'N' (not branch)
			// The requester should act just like with an error code 'E'
			printf("Problem in the DBT. We miss a BranchNode somewhere.\n Problem detected in KDp2p_SubscribeToNodeDialog::OnQuestion. Sent error code 'N' to %s\n",answer->GetAddress()->ToString().c_str());
			answer->AddChar('N');
			return;
		}*/

		currentPosDepth = currentNode->GetPosition().GetNBBits();
		// ok, who is the child node on the opposite corner of the tree we want?
		bool direction = goalPos.GetBit(currentPosDepth/*+1*/);
		vector<KDp2p_NetworkAddress> nodeConnections;
		((KDp2p_BranchNode*)currentNode)->GetChild(!direction)->PickRandomAddress(nodeConnections, bTree->GetNbIPsForConnectionNodes());
	
		answer->AddChar((char) nodeConnections.size());
		KD_LogFile::printf2("Ajoute un char: %d\n", nodeConnections.size());
		vector<KDp2p_NetworkAddress>::iterator it = nodeConnections.begin();
		while (it != nodeConnections.end())
		{
			(*it).AddToMessage(answer);
			KD_LogFile::printf2("Ajoute une addresse: %s\n",(*it).ToString().c_str());
			it++;
		}
		// ok, done, we added it. Let's do it again with next node.
		currentNode = ((KDp2p_BranchNode*)currentNode)->GetChild(direction);
		// Oh! if next node is a ConnectionNode, we are done.
	} while (currentNode->GetNodeType() == BranchNodeType);
	
	// Let's first declare the end with a -1 instead of the number of IPs sent.	
	answer->AddChar(-1);
	KD_LogFile::printf2("Ajoute un char: -1\n");
	
	// Ok, now let's add the connectionNode we need to continue.
	deque<KDp2p_NetworkAddress> lastNodeConnections;
	// set all the addresses in this node
	((KDp2p_ConnectionNode*)currentNode)->GetAllAddresses(lastNodeConnections);
	//((KDp2p_ConnectionNode*)currentNode)->PickRandomAddress(lastNodeConnections, bTree->GetNbIPsForConnectionNodes());
	
	answer->AddChar((char) lastNodeConnections.size());
	KD_LogFile::printf2("Ajoute un char: %d\n", lastNodeConnections.size());
	deque<KDp2p_NetworkAddress>::iterator it = lastNodeConnections.begin();
	while (it != lastNodeConnections.end())
	{
		(*it).AddToMessage(answer);
		KD_LogFile::printf2("Ajoute une addresse: %s\n",(*it).ToString().c_str());
		it++;
	}

	// We are done!!!
	SendAnswer();
	
	// Then maybe add the requester IP to the DBT
	// In fact, we DO add it. And we even add it at the beginning.
	((KDp2p_ConnectionNode*)currentNode)->AddNetworkAddress(*(question->GetAddress()));

	// That's all folks!
}

void KDp2p_SubscribeToNodeDialog::SendQuestion()
{
	// The nodes we want to subscribe to
	question->AddInt(treeID);
	nodePos.AddToMessage(question);
	goalPos.AddToMessage(question);

	SUPER::SendQuestion();
}

void KDp2p_SubscribeToNodeDialog::SetNodeToRetrieve(unsigned int treeID, const KDp2p_BPosition &pos, const KDp2p_BPosition &goal, KDp2p_SerializableObject *object)
{
	this->treeID = treeID;
	nodePos = pos;
	goalPos = goal;
	this->object = object;

	// DEBUG CODE!
	if (!goalPos.IsAChildOf(pos))
	{
		printf("WARNING! In KDp2p_SubscribeToNodeDialog::SetNodeToRetrieve, we are going to send a buggy set of nodePos / goalPos!\n");
		printf("goalPos %s\n", goalPos.ToString().c_str());
		printf("nodePos %s\n", pos.ToString().c_str());
	}
}

void KDp2p_SubscribeToNodeDialog::AddAlternativePeer(const KDp2p_NetworkAddress &alternativePeer)
{
	alternativePeers.push_back(alternativePeer);
}

void KDp2p_SubscribeToNodeDialog::AskAlternativePeer()
{
	KDp2p_BTree *bTree = (KDp2p_BTree *) GetEngine()->GetUniqueObject(treeID);

	KDp2p_BNode *nodeAsked;
	KDp2p_BPosition nodeAskedPos = KDp2p_BPosition();

	// checks there is at least one node in the tree
	if (bTree->GetRootNode() != 0)
	{
		nodeAsked = bTree->FindClosestNode(nodePos);

		if (nodeAsked->GetPosition() != nodePos)
		{
			// The node we asked is not even part of the tree anymore!
			printf("Furthermore, the node we asked is not even part of our tree anymore!");
		}
		else if (nodeAsked->GetNodeType() != ConnectionNodeType)
		{
			// The node we asked is not even a ConnectionNode anymore!
			printf("Furthermore, the node we asked is not even a ConnectionNode anymore.");
			nodeAsked = bTree->FindClosestNode(goalPos);
			if (nodeAsked->GetPosition() == goalPos)
				return;
		}
		else
		{
			// Normal error processing : add the node to the black list
			((KDp2p_ConnectionNode *) nodeAsked)->AddToBlackList((*answer->GetAddress()));
		}

		// Ok, let's ask the question to someone else then, if we can!
		if (alternativePeers.empty())
		{
			// if we can't, let's suscribe the object directly!
			printf("Failed to suscribe the node from peers information. Node suscribed directly without peers!");

			bTree->AddLeafDirectly(nodeAsked->GetParent(), goalPos, object);
			return;
		}

		nodeAskedPos = nodeAsked->GetPosition();
	}
	else
	{
		// if there is no node in the tree
		if (alternativePeers.empty())
		{
			// if we can't, let's suscribe the object directly!
			printf("Failed to suscribe the node from peers information. Node suscribed directly without peers!");

			bTree->AddLeafDirectly(0, goalPos, object);
			return;
		}
	}

	// ok, we can ask alternativePeers.
	KDp2p_SubscribeToNodeDialog *newDialog = new KDp2p_SubscribeToNodeDialog(GetEngine());
	newDialog->SetNodeToRetrieve(treeID, nodeAskedPos, goalPos, object);

	deque<KDp2p_NetworkAddress>::iterator itAlternativePeers = alternativePeers.begin();

	newDialog->GetQuestion()->SetAddress(*itAlternativePeers);
	itAlternativePeers++;

	while (itAlternativePeers != alternativePeers.end())
	{
		newDialog->AddAlternativePeer(*itAlternativePeers);
		itAlternativePeers++;
	}
	newDialog->SendQuestion();		

	return;
}
