#ifndef AllPeers_H
#define AllPeers_H

#include "../Tools/defines.h"

#include "../Network/NetworkAddress.h"

#include <map>
#include <string>
using namespace std;

#include <time.h>
#include <sys/timeb.h>

/**
	Contains the list of all peers ever contacted, with the last date when they have been contacted
*/
class DllExport KDp2p_AllPeers
{

	/////// ICI il faut un double SET (pour les indexs).
	/////// Bon y a pb avec l'opérateur de comparaison... zutouille...
	/////// On va mettre 2 map, mais faudrait que je prévois un objet index, ça serait cool
	map<KDp2p_NetworkAddress, time_t> peerListByIP;
	multimap<time_t, KDp2p_NetworkAddress> peerListByTime;

	multimap<time_t, KDp2p_NetworkAddress>::reverse_iterator itByTime;

public:	
	KDp2p_AllPeers();
	virtual ~KDp2p_AllPeers();

	/// Finds if the given address is in the list of our peers
	bool IsAPeer(KDp2p_NetworkAddress *address);

	/// Adds the peer to the list. If the peer exists, updates it.
	void AddPeer(KDp2p_NetworkAddress *address);	

	/**
		Returns a peer (classified by date from the most current to the oldest)
		Pointers are reseted when a Peer is added.
	*/
	KDp2p_NetworkAddress GetPeer();

	/// Move the pointer to the next peer in the list. Returns false when we reach the end (a reversed iterator is used for this list).
	bool Next();

	/// Resets the pointer
	void ResetPointer();

	/// Saves the list of peers
	void Save(const string &filename);

	/// Loads the list of peers
	void Load(const string &filename);
};

#endif
