#ifndef PingDialog_H
#define PingDialog_H

#include "../Tools/defines.h"

#include "Dialog.h"

/**
	A PING and the associated PONG as answer.
*/
class DllExport KDp2p_PingDialog : public KDp2p_Dialog
{
	

public:
	KDp2p_PingDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message);
	virtual ~KDp2p_PingDialog();

	/// Method called when the answer is longer than timeOut to arrive.
	virtual void OnTimeOut();

	/// Method called on reception of the answer
	virtual void OnAnswer();

	/// Method called on reception of the question
	virtual void OnQuestion(KDp2p_DialogFactory *factory);
};

#endif
