#include "AllPeers.h"

#include "../Tools/NetException.h"

#include <stdio.h>

KDp2p_AllPeers::KDp2p_AllPeers()
{
	itByTime = peerListByTime.end();
	itByTime--;
}

KDp2p_AllPeers::~KDp2p_AllPeers()
{

}

bool KDp2p_AllPeers::IsAPeer(KDp2p_NetworkAddress *address)
{
	if (peerListByIP.find(*address) != peerListByIP.end())
		return true;
	else
		return false;
}

void KDp2p_AllPeers::AddPeer(KDp2p_NetworkAddress *address)
{
	map<KDp2p_NetworkAddress, time_t>::iterator it = peerListByIP.find(*address);
	// if the peer already exists, lets first delete it in the multimap
	if (it != peerListByIP.end())
	{
		time_t t_old = it->second;
		multimap<time_t, KDp2p_NetworkAddress>::iterator it2 = peerListByTime.find(t_old);
		while (!(it2->second == *address))
		{
			it2++;
			if (it2 == peerListByTime.end())
				throw new KDp2p_NetException("Date not found in list!!! This should never happen!");
		}
		peerListByTime.erase(it2);
	}

	time_t t;
	time(&t);

	peerListByIP[*address] = t;
	peerListByTime.insert(pair<time_t, KDp2p_NetworkAddress> (t, *address));

	itByTime = peerListByTime.end();
	itByTime--;
}

KDp2p_NetworkAddress KDp2p_AllPeers::GetPeer()
{
	return itByTime->second;
}

bool KDp2p_AllPeers::Next()
{
	itByTime--;
	if (itByTime == peerListByTime.begin())
		return false;
	else
		return true;
}

void KDp2p_AllPeers::ResetPointer()
{
	itByTime = peerListByTime.end();
	itByTime--;
}

void KDp2p_AllPeers::Save(const string &filename)
{
	FILE *fpt = fopen(filename.c_str(), "wb");
	if (fpt==0)
	{
		throw KDp2p_NetException("Unable to open file in write mode");
	}

	//KDp2p_P2PEngine::GetSingeton()->GetVersion()
	//KDp2p_P2PEngine::GetSingeton()->GetMaxPeerNumberInFile()
	unsigned int version = 1;
	unsigned int maxPeerNumber = 5000;
	unsigned int count = (unsigned int) peerListByTime.size();

	char magicNumber[5]="KDAP"; // KD ALL PEERS

	fwrite(magicNumber, sizeof(char)*4, 1, fpt);
	fwrite(&version, sizeof(unsigned int), 1, fpt);
	fwrite(&count, sizeof(unsigned int), 1, fpt);

	multimap<time_t, KDp2p_NetworkAddress>::iterator saveIt = peerListByTime.begin();
	
	int nb=0;

	while (saveIt != peerListByTime.end() && maxPeerNumber != nb)
	{
		saveIt->second.WriteToFile(fpt);
		fwrite(&(saveIt->first), sizeof(time_t), 1, fpt);
		saveIt++;
		nb++;
	}

	fclose(fpt);
}

void KDp2p_AllPeers::Load(const string &filename)
{
	FILE *fpt = fopen(filename.c_str(), "rb");
	if (fpt==0)
	{
		throw KDp2p_NetException("Unable to open file in read mode");
	}

	//KDp2p_P2PEngine::GetSingeton()->GetVersion()
	unsigned int version;
	unsigned int expectedVersion = 1;
	unsigned int count;

	char magicNumber[5]; // KD ALL PEERS
	magicNumber[4]=0;
	int ret = (int) fread(magicNumber, sizeof(char)*4, 1, fpt);

	if (ret == 0)
		throw KDp2p_NetException("Unable to read from file\n");

	if (strcmp(magicNumber,"KDAP"))
		throw KDp2p_NetException("Invalid File Format\n");

	ret = (int) fread(&version, sizeof(unsigned int), 1, fpt);
	if (ret == 0)
		throw KDp2p_NetException("Unable to read from file\n");

	if (version != expectedVersion)
		throw KDp2p_NetException("File version not correct\n");

	ret = (int) fread(&count, sizeof(unsigned int), 1, fpt);
	if (ret == 0)
		throw KDp2p_NetException("Unable to read from file\n");


	for (unsigned int i=0; i<count; i++)
	{
		KDp2p_NetworkAddress address;
		address.ReadFromFile(fpt);
	
		time_t time;
		ret = (int) fread(&time, sizeof(time_t), 1, fpt);
		if (ret == 0)
			throw KDp2p_NetException("Unable to read from file");

		peerListByIP[address] = time;
		peerListByTime.insert(pair<time_t, KDp2p_NetworkAddress> (time, address));
	}

	fclose(fpt);

	itByTime = peerListByTime.end();
	itByTime--;
}
