#include "AllPeers.h"

#include "../Tools/NetException.h"

#include <stdio.h>

// Note : this macro should be moved somewhere else
#define SWAP32(x)                                     \
{ unsigned int i1, i2, i3, i4;                        \
  c1 = i & 0xFF;                                      \
  c2 = i & 0xFF00;                                    \
  c3 = i & 0xFF0000;                                  \
  c4 = i & 0xFF000000;                                \
  x = (c1 << 24) | (c2 << 8) | (c3 >> 8) | (c4 >> 24) \
}

KDp2p_AllPeers::KDp2p_AllPeers()
{
    itByTime = peerListByTime.rbegin();
    //itByTime = peerListByTime.end();
    //itByTime--;
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

    /*itByTime = peerListByTime.end();
    itByTime--;*/
    itByTime = peerListByTime.rbegin();
}

KDp2p_NetworkAddress KDp2p_AllPeers::GetPeer()
{
    return itByTime->second;
}

bool KDp2p_AllPeers::Next()
{
    /*itByTime--;
    if (itByTime == peerListByTime.begin())
        return false;
    else
        return true;*/
    itByTime++;
    if (itByTime == peerListByTime.rend())
        return false;
    else
        return true;
}

void KDp2p_AllPeers::ResetPointer()
{
    /*itByTime = peerListByTime.end();
    itByTime--;*/
    itByTime = peerListByTime.rbegin();
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

    unsigned int nb = 0;

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
        throw KDp2p_NetException(
            "Unable to open file `" + filename + "' in read mode");

	//KDp2p_P2PEngine::GetSingeton()->GetVersion()
	unsigned int version;
	unsigned int expectedVersion = 1;
	unsigned int count;


    char magicNumber[5]; // KD ALL PEERS
    magicNumber[4]=0;
    int ret = (int) fread(magicNumber, 4, 1, fpt);
#ifdef MSB
    SWAP32(magicNumber);
#endif
    if (ret == 0)
        throw KDp2p_NetException("Unable to read file `" + filename + "'\n");

    if (strcmp(magicNumber, "KDAP"))
        throw KDp2p_NetException(filename+ ": Invalid file format\n");



	ret = (int) fread(&version, sizeof(unsigned int), 1, fpt);
	if (ret == 0)
		throw KDp2p_NetException("Unable to read from file\n");

	if (version != expectedVersion)
		throw KDp2p_NetException("File version not correct\n");



    ret = (int) fread(&count, 4, 1, fpt);
    if (ret == 0)
        throw KDp2p_NetException("Unable to read file `" + filename + "'\n");
#ifdef MSB
    SWAP32(count);
#endif


	 for (unsigned int i= 0; i< count; i++)
    {
        KDp2p_NetworkAddress address;
        address.ReadFromFile(fpt);

        unsigned int temp_time;
        ret = (int) fread(&temp_time, 4, 1, fpt);
        if (ret == 0)
            throw KDp2p_NetException("Unable to read from file");
#ifdef MSB
        SWAP32(temp_time);
#endif

        peerListByIP[address] = (time_t) temp_time;
        peerListByTime.insert(pair<time_t, KDp2p_NetworkAddress>
            ((time_t) temp_time, address));
    }

	    
    //KDp2p_P2PEngine::GetSingleton()->GetVersion()
}
