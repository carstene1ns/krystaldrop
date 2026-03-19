#ifndef SubscribeToNodeDialogFactory_H
#define SubscribeToNodeDialogFactory_H

#include "../../Tools/defines.h"

#include "../DialogFactory.h"

class KDp2p_P2PEngine;
class KDp2p_Dialog;

/**
	
*/
class DllExport KDp2p_SubscribeToNodeDialogFactory : public KDp2p_DialogFactory
{
public:
	KDp2p_SubscribeToNodeDialogFactory();
	virtual ~KDp2p_SubscribeToNodeDialogFactory();

	/// Method called to create the SubscribeToNode dialog.
	virtual KDp2p_Dialog *NewDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message, int questionType);
};

#endif
