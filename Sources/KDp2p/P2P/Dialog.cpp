#include "Dialog.h"

#include "../Network/Message.h"
#include "DialogManager.h"
#include "P2PEngine.h"
#include "../Network/SentMessageQueue.h"

#include <SDL/SDL.h>

unsigned int KDp2p_Dialog::currentId = 0;

KDp2p_Dialog::KDp2p_Dialog(KDp2p_P2PEngine *engine, unsigned int questionType) : answer(0)
{
	InitQuestionDialog(engine, questionType);
	isAnswering = false;
}

KDp2p_Dialog::KDp2p_Dialog(KDp2p_P2PEngine *engine, char questionTypeChar[5]) : answer(0)
{
	unsigned int questionType = (questionTypeChar[0]<<24) + (questionTypeChar[1]<<16) + (questionTypeChar[2]<<8) + questionTypeChar[3];
	InitQuestionDialog(engine, questionType);
	isAnswering = false;
}

KDp2p_Dialog::KDp2p_Dialog(KDp2p_P2PEngine *engine, KDp2p_Message *question)
{
	this->engine = engine;
	// We assume that the "QUES" word has been read, the further type has been read too, but that the cursor is just after.
	this->question = question;
	answer = new KDp2p_Message();
	answer->AddChar('A');
	answer->AddChar('N');
	answer->AddChar('S');
	answer->AddChar('W');
	answer->AddInt(question->GetInt());

	isAnswering = true;
}

KDp2p_Dialog::~KDp2p_Dialog()
{
	// WARNING THE QUESTION IS DELETED JUST AFTER IT HAS BEEN SENT
	if (question)
		delete question;
	if (answer && !isAnswering)
		delete answer;
}

void KDp2p_Dialog::InitQuestionDialog(KDp2p_P2PEngine *engine, unsigned int questionType)
{
	this->engine = engine;

	// By default, timeout is set to the ConnectionTimeOut of P2PEngine
	timeOut = engine->GetConnectionTimeOut();

	questionId = currentId;

	currentId++;
	
    question = new KDp2p_Message();
	question->AddChar('Q');
	question->AddChar('U');
	question->AddChar('E');
	question->AddChar('S');
	question->AddInt(questionType);
	question->AddInt(questionId);
	
}

void KDp2p_Dialog::SetAnswer(KDp2p_Message *answer)
{
	this->answer = answer;
}

KDp2p_Message *KDp2p_Dialog::GetQuestion()
{
	return question;
}

unsigned int KDp2p_Dialog::GetQuestionId()
{
	return questionId;
}

void KDp2p_Dialog::SetTimeOut(int timeOut)
{
	this->timeOut = timeOut;
}

int KDp2p_Dialog::GetTimeToDiscard()
{
	return timeToDiscard;
}

void KDp2p_Dialog::SendQuestion()
{
	timeToDiscard = SDL_GetTicks() + timeOut;
	engine->GetSendQueue()->AddMessage(question);
	engine->GetDialogManager()->AddDialog(this);
	question = 0;
}

void KDp2p_Dialog::SendAnswer()
{
	engine->GetSendQueue()->AddMessage(answer);
}
