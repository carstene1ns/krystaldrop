#include "SubscribeToNodeDialogFactory.h"

#include "SubscribeToNodeDialog.h"

KDp2p_SubscribeToNodeDialogFactory::KDp2p_SubscribeToNodeDialogFactory()
{

}

KDp2p_SubscribeToNodeDialogFactory::~KDp2p_SubscribeToNodeDialogFactory()
{

}

KDp2p_Dialog *KDp2p_SubscribeToNodeDialogFactory::NewDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message, int questionType)
{
	return new KDp2p_SubscribeToNodeDialog(engine,message,questionType);
}
