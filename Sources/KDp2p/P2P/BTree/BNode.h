#ifndef BNode_H
#define BNode_H

#include "../../Tools/defines.h"


/**
	The base class for binary nodes.
*/
class DllExport KDp2p_BNode
{
protected:
	(KDp2p_BNode*) children[2];

public:

	KDp2p_BNode();

	virtual ~KDp2p_BNode();

	/**
		Returns the specified child
	*/
	KDp2p_BNode *GetChild(bool child) const;

	/**
		Sets the specified child, if the node was previously allocated, it is deleted.
	*/
	void SetChild(bool child, KDp2p_BNode *node);
};

#endif
