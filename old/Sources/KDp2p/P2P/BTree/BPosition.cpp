#include "BPosition.h"

#include "../../Network/Message.h"

KDp2p_BPosition::KDp2p_BPosition() : maxLevel(0)
{

}

KDp2p_BPosition::KDp2p_BPosition(const KDp2p_BPosition &that)
{
	vector<unsigned int>::const_iterator it = that.pos.begin();

	while (it != that.pos.end())
	{
		pos.push_back(*it);
		it++;
	}

	maxLevel = that.maxLevel;
}

KDp2p_BPosition::KDp2p_BPosition(const KDp2p_P2PId &id)
{
	pos.push_back(id.id[0]);
	pos.push_back(id.id[1]);
	pos.push_back(id.id[2]);
	pos.push_back(id.id[3]);
	maxLevel=128;
}

KDp2p_BPosition::~KDp2p_BPosition()
{

}

bool KDp2p_BPosition::operator == (const KDp2p_BPosition &that) const
{
	// If they have not the same number of bits, they are different
	if (that.maxLevel != maxLevel)
		return false;

	//for (int i=0; i<((int)pos.size())-1; i++)
	for (int i=0; i<maxLevel/32; i++)
		if (pos[i] != that.pos[i])
			return false;
	
	if (maxLevel%32 == 0)
		return true;

	// last int number
	int last1 = pos[maxLevel/32] & ((1<<(maxLevel%32))-1);
	int last2 = that.pos[maxLevel/32] & ((1<<(maxLevel%32))-1);

	if (last1 != last2)
		return false;

	return true;
}

bool KDp2p_BPosition::operator != (const KDp2p_BPosition &that) const
{
	return !((*this)==that); 
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

void KDp2p_BPosition::AddToMessage(KDp2p_Message *message)
{
	// Note: maxLevel is cast in Short Int to take less space on the network
	message->AddShortInt(maxLevel);
	if (maxLevel==0)
		return;
	for (int i=0; i<=(maxLevel-1)/32; i++)
	{
		message->AddInt(pos[i]);
	}
}

void KDp2p_BPosition::ReadFromMessage(KDp2p_Message *message)
{
	// Mise à zéro:
	pos.clear();

	maxLevel = message->GetShortInt();

	if (maxLevel==0)
		return;
	for (int i=0; i<=(maxLevel-1)/32; i++)
	{
		pos.push_back(message->GetInt());
	}
}

bool KDp2p_BPosition::IsAChildOf(const KDp2p_BPosition &parent)
{
	if (maxLevel < parent.maxLevel)
		return false;

	// compare all the elements of the 2 positions to parent.maxLevel

	// First for the "full ints"
	for (int i=0; i<parent.maxLevel/32; i++)
	{
		if (pos[i] != parent.pos[i])
			return false;
	}

	if (parent.maxLevel<32 || parent.maxLevel%32 == 0)
		return true;

	// Now for the remaining int with a mask
	//unsigned int mask = (1<<((parent.maxLevel%32)+1)) - 1;
	unsigned int mask = (1<<(parent.maxLevel%32)) - 1;

	if ((pos[parent.pos.size()-1] & mask) != (parent.pos[parent.pos.size()-1] & mask))
		return false;

	return true;
}

void KDp2p_BPosition::RemoveBit()
{
	if (maxLevel<=0)
		return;

	maxLevel--;

	if (maxLevel%32 == 0)
		pos.pop_back();
}

string KDp2p_BPosition::ToString() const
{
	string str;

	for (int i=0; i<maxLevel; i++)
	{
		if (GetBit(i)==false)
			str += "0";
		else
			str += "1";
	}
	return str;
}
