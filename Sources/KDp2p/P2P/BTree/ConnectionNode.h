#ifndef ConnectionNode_H
#define ConnectionNode_H

#include "../../Tools/defines.h"

#include "BNode.h"

/**
	A leaf in the Distributed P2P tree, used to gain access to the rest of the tree from the P2P network.
*/
class DllExport KDp2p_ConnectionNode : public KDp2p_BNode
{

public:

	KDp2p_ConnectionNode();

	virtual ~KDp2p_ConnectionNode();

	
};

#endif
