#ifndef DialogManager_H
#define DialogManager_H

#include "../Tools/defines.h"

#include "MessageHandler.h"

#include <deque>
#include <map>
using namespace std;

class KDp2p_Dialog;
class KDp2p_Message;
class KDp2p_DialogFactory;
class KDp2p_P2PEngine;

#define QUES_MESSAGEID ((((('Q'<<24) + ('U'<<16) + ('E'<<8) + 'S'))))
#define ANSW_MESSAGEID ((((('A'<<24) + ('N'<<16) + ('S'<<8) + 'W'))))

/**
	Set of questions and the associated answers.
	The DialogManager class handles TimeOut of answers
	and which class should be created to answer the questions
*/
class DllExport KDp2p_DialogManager : public KDp2p_MessageHandler
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

	/**
		On exit, removes all the unfinished dialogs
		but does not delete all factories
	*/
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

	/**
		Removes a given question before any answer has been received
	*/
	bool RemoveQuestion(KDp2p_Dialog *dialog);

	//{
	/**
		Removes all the non answered questions from a given type
	*/
	void RemoveQuestionsByType(char idChar[5]);
	void RemoveQuestionsByType(int id);
	//}

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
		Removes a factory but DOES NOT delete it!
	*/
	void RemoveFactory(int id);
	void RemoveFactory(char idChar[5]);
	//}

	//{
	/**
		Removes a factory AND delete it!
	*/
	void RemoveAndDeleteFactory(int id);
	void RemoveAndDeleteFactory(char idChar[5]);
	//}

	/**
		Methods that handle the messages received --> throw ProcessAnswer or ProcessQuestion depending on message type
	*/
	virtual void HandleMessage(KDp2p_Message *message, int id);
};

#endif
