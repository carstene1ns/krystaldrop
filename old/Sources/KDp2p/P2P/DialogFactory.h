#ifndef DialogFactory_H
#define DialogFactory_H

#include "../Tools/defines.h"

class KDp2p_P2PEngine;
class KDp2p_Dialog;
class KDp2p_Message;

/**
	The DialogFactory is an abstract class.
	The inherited class can instantiate dialog classes with the NewDialog() class
	Furthermore, the factory can be used as an entry point for the Dialogs to access data from the application.
*/
class DllExport KDp2p_DialogFactory
{
public:
	KDp2p_DialogFactory();
	virtual ~KDp2p_DialogFactory();

	/// Method called to create the dialog.
	virtual KDp2p_Dialog *NewDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message, int questionType) = 0;
};

#endif
