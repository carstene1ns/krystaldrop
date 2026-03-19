#ifndef BPosition_H
#define BPosition_H

#include "../../Tools/defines.h"

#include <vector>
using namespace std;

class KDp2p_Message;

#include "../P2PId.h"

/**
	A position in the binary tree
*/
class DllExport KDp2p_BPosition
{
protected:
	/// The bit field
	vector<unsigned int> pos;

	/// The level in the bitfield (0 = position of the root node)
	int maxLevel;

public:

	/**
		Creates an empty binary position
	*/
	KDp2p_BPosition();

	/**
		Copy constructor
	*/
	KDp2p_BPosition(const KDp2p_BPosition &that);

	/**
		Constructor from P2PId
	*/
	KDp2p_BPosition(const KDp2p_P2PId &id);

	virtual ~KDp2p_BPosition();

	/**
		Operator ==
	*/
	bool operator == (const KDp2p_BPosition &that) const;

	/**
		Operator !=
	*/
	bool operator != (const KDp2p_BPosition &that) const;

	/**
		Returns the number of bits (maxLevel)
	*/
	int GetNBBits() const;

	/**
		Adds a bit at the end of the BPosition
	*/
	void AddBit(bool bit);

	/**
		Returns the value of the bit at level "level"
	*/
	bool GetBit(int level) const;

	/**
		Add the BPosition to a Message
	*/
	void AddToMessage(KDp2p_Message *message);

	/**
		Read the BPosition from a Message
	*/
	void ReadFromMessage(KDp2p_Message *message);

	/**
		Returns true if this position is a child of the given position
		If the 2 positions are equal, it will also return true
	*/
	bool IsAChildOf(const KDp2p_BPosition &parent);

	/**
		Removes a bit at the end of the BPosition
		(go up one level in the tree)
	*/
	void RemoveBit();

	/**
		Displays the BPosition
	*/
	string ToString() const;
};

#endif
