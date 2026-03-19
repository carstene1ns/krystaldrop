#ifndef PingDialogFactory_H
#define PingDialogFactory_H

#include "../Tools/defines.h"

#include "DialogFactory.h"

class KDp2p_P2PEngine;
class KDp2p_Dialog;

/**
	The DialogFactory is an abstract class.
	The inherited class can instantiate dialog classes with the NewDialog() class
*/
class DllExport KDp2p_PingDialogFactory : public KDp2p_DialogFactory
{
public:
	KDp2p_PingDialogFactory();
	virtual ~KDp2p_PingDialogFactory();

	/// Method called to create the ping dialog.
	virtual KDp2p_Dialog *NewDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message, int questionType);
};

#endif
