#include "PingDialogFactory.h"

#include "PingDialog.h"

KDp2p_PingDialogFactory::KDp2p_PingDialogFactory()
{

}

KDp2p_PingDialogFactory::~KDp2p_PingDialogFactory()
{

}

KDp2p_Dialog *KDp2p_PingDialogFactory::NewDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message)
{
	return new KDp2p_PingDialog(engine,message);
}
