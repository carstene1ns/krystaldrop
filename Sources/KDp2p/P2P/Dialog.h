#ifndef Dialog_H
#define Dialog_H

#include "../Tools/defines.h"

class KDp2p_Message;
class KDp2p_DialogManager;
class KDp2p_DialogFactory;
class KDp2p_P2PEngine;
class KDp2p_SentMessageQueue;

/**
	A question and the associated answer.
	The DialogManager class handles TimeOut of answers
*/
class DllExport KDp2p_Dialog
{
	friend class KDp2p_DialogManager;
protected:
	/// The question asked
	KDp2p_Message *question;
	/// The answer received
	KDp2p_Message *answer;

	/// The type of the dialog
	int questionType;

private:
	/// Timeout before discarding the question (time in milliseconds)
	int timeOut;

	/// Timeout before discarding the question (time in milliseconds)
	int timeToDiscard;

	/// The Id of the question. The Id is unique for a given question
	unsigned int questionId;

	/// The next question Id
	static unsigned int currentId;

	/// Pointer to the engine
	KDp2p_P2PEngine *engine;

	/// True is this class is answering a question. False otherwise
	bool isAnswering;

	/**
		Construct the question (called by the constructor)
	*/
	void InitQuestionDialog(KDp2p_P2PEngine *engine, unsigned int questionType);

public:
	//{
	/// Constructor used when initiating the dialog
	KDp2p_Dialog(KDp2p_P2PEngine *engine, char questionTypeChar[5]);
	KDp2p_Dialog(KDp2p_P2PEngine *engine, unsigned int questionType);
	//}

	/// Constructor used when receiving the question
	KDp2p_Dialog(KDp2p_P2PEngine *engine, KDp2p_Message *question, int questionType);

	/**
		Destructor of the Dialog.
		The message sent and recived are deleted!
		WARNING! the Dialog becomes the OWNER of the messages!
	*/
	virtual ~KDp2p_Dialog();

	/// Method called when the answer is longer than timeOut to arrive.
	virtual void OnTimeOut() = 0;

	/// Method called on reception of the answer
	virtual void OnAnswer() = 0;

	/**
		Method called on reception of the question
		The factory should be used to access "world" data
	*/
	virtual void OnQuestion(KDp2p_DialogFactory *factory) = 0;

	/// Sets the answer. Called by DialogManager
	void SetAnswer(KDp2p_Message *answer);

	/// Returns the question so that it can be filled
	KDp2p_Message *GetQuestion();

	/// Returns the question id
	unsigned int GetQuestionId();

	/// Returns the question type
	unsigned int GetQuestionType();

	/// Sets the timeout before discarding the question (time in milliseconds)
	void SetTimeOut(int timeOut);

	/// Returns the time at which the dialog should be discarded
	int GetTimeToDiscard();

	/*
		Sends the Question, the question message is deleted just after being sent
		The dialog is added to the DialogManager, from that point, we mustn't delete the dialog.
		It will be deleted by the DialogManager, after the OnAnswer method is executed.
	*/
	virtual void SendQuestion();

	/*
		Sends the Answer
	*/
	void SendAnswer();
};

#endif
