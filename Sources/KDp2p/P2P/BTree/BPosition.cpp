#include "BPosition.h"

KDp2p_BPosition::KDp2p_BPosition() : maxLevel(0)
{

}

KDp2p_BPosition::KDp2p_BPosition(const KDp2p_BPosition &that)
{
	vector<int>::const_iterator it = that.pos.begin();

	while (it != that.pos.end())
	{
		pos.push_back(*it);
		it++;
	}

	maxLevel = that.maxLevel;
}

KDp2p_BPosition::~KDp2p_BPosition()
{

}

int KDp2p_BPosition::GetNBBits() const
{
	return maxLevel;
}

void KDp2p_BPosition::AddBit(bool bit)
{
	if (maxLevel%32 == 0)
		pos.push_back(0);

	/// Sets the given bit to 0
	if (!bit)
		pos[maxLevel/32] &= ~(1<<(maxLevel%32));
	/// Sets the given bit to 1
	else
		pos[maxLevel/32] |= (1<<(maxLevel%32));

	maxLevel++;
}

bool KDp2p_BPosition::GetBit(int level) const
{
	return (pos[level/32] & (1<<(level%32))) != 0;
}
