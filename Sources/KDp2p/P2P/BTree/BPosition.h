#ifndef BPosition_H
#define BPosition_H

#include "../../Tools/defines.h"

#include <vector>
using namespace std;

/**
	A position in the binary tree
*/
class DllExport KDp2p_BPosition
{
protected:
	/// The bit field
	vector<int> pos;

	/// The level in the bitfield
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

	virtual ~KDp2p_BPosition();

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

};

#endif
