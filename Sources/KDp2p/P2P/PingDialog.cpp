#include "PingDialog.h"

#include "../Network/Message.h"
#include "../Network/NetworkAddress.h"
#include "../../KDpp/Tools/LogFile.h"

KDp2p_PingDialog::KDp2p_PingDialog(KDp2p_P2PEngine *engine) : KDp2p_Dialog(engine, "PING"), pingCounter(0)
{
	
}

KDp2p_PingDialog::KDp2p_PingDialog(KDp2p_P2PEngine *engine, KDp2p_Message *message) : KDp2p_Dialog(engine, message)
{

}

KDp2p_PingDialog::~KDp2p_PingDialog()
{

}

void KDp2p_PingDialog::OnTimeOut()
{
	// OK THE FACTORY SHOULD BE USED TO CREATE ANY KIND OF DIALOG!!!!
}

void KDp2p_PingDialog::OnAnswer()
{
	// OK THE FACTORY SHOULD BE USED TO CREATE ANY KIND OF DIALOG!!!!
	// Why? Can't remember... if it is to get a pointer to the factory
	// that would act as a center, is it necessary?
	// Why not developping a PingCounter class --> this sound cool.
	KD_LogFile::printf2("Received answer from the PING request to %s\n",answer->GetAddress()->ToString().c_str());
	
	
}

void KDp2p_PingDialog::OnQuestion(KDp2p_DialogFactory *factory)
{
	KD_LogFile::printf2("Received a PING question from %s\n",question->GetAddress()->ToString().c_str());
		
	// Let's send the answer!
	answer->AddChar('P');
	answer->AddChar('O');
	answer->AddChar('N');
	answer->AddChar('G');
	SendAnswer();
}
