#ifndef DialogManager_H
#define DialogManager_H

#include "../Tools/defines.h"

#include <deque>
#include <map>
using namespace std;

class KDp2p_Dialog;
class KDp2p_Message;
class KDp2p_DialogFactory;
class KDp2p_P2PEngine;

/**
	A question and the associated answer.
	The DialogManager class handles TimeOut of answers
*/
class DllExport KDp2p_DialogManager
{
	/**
		List of dialogs for which we are waiting for an answer
	*/
	deque <KDp2p_Dialog *> currentDialogs;

	/**
		Maps the id of the dialog to a factory that will be the right kind of dialog.
		Used when creating dialogs for answers
	*/
	map <int, KDp2p_DialogFactory *> factories;

	KDp2p_P2PEngine *engine;

public:
	KDp2p_DialogManager(KDp2p_P2PEngine *engine);
	virtual ~KDp2p_DialogManager();

	/// Used by each Dialog
	void AddDialog(KDp2p_Dialog *dialog);
	
	/**
		Process an answer message (a message starting with "ANSW")
	*/
	void ProcessAnswer(KDp2p_Message *message);

	/**
		Process a question message (a message starting with "QUES")
		Creates the appropriate Dialog message using the appropriate DialogFactory and run the OnQuestion method
	*/
	void ProcessQuestion(KDp2p_Message *message);

	void ProcessTimeOut();

	//{
	/**
		Adds a factory to the manager
		The factory is used to create the right kind of object when a question is received
	*/
	void AddFactory(int id, KDp2p_DialogFactory *factory);
	void AddFactory(char idChar[5], KDp2p_DialogFactory *factory);
	//}

	//{
	/**
		Removes a factory
	*/
	void RemoveFactory(int id);
	void RemoveFactory(char idChar[5]);
	//}
};

#endif
