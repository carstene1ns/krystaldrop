#include "DialogManager.h"

#include "../Network/Message.h"
#include "../Network/NetworkAddress.h"
#include "Dialog.h"
#include "DialogFactory.h"
#include "../../KDpp/Tools/Logfile.h"


#include <SDL/SDL.h>

KDp2p_DialogManager::KDp2p_DialogManager(KDp2p_P2PEngine *engine)
{
	this->engine = engine;
}

KDp2p_DialogManager::~KDp2p_DialogManager()
{
	// Should be tested for threads....

	// First deletes all dialogs
	deque<KDp2p_Dialog *>::iterator it = currentDialogs.begin();

	while (it != currentDialogs.end())
	{
		delete *it;
		it++;
	}
	currentDialogs.clear();

	// Then remove all factories
	// Mmmm maybe not.
}

void KDp2p_DialogManager::AddDialog(KDp2p_Dialog *dialog)
{
	currentDialogs.push_back(dialog);
}

void KDp2p_DialogManager::ProcessAnswer(KDp2p_Message *message)
{
	int answerId = message->GetInt();

	deque<KDp2p_Dialog *>::iterator it = currentDialogs.begin();

	while (it != currentDialogs.end())
	{
		if (answerId == (*it)->GetQuestionId())
			break;

		it++;
	}

	if (it == currentDialogs.end())
	{
		KD_LogFile::printf2("Problem: could not find question to given answer: the question must have timed out too early\n");
		return;
	}

	(*it)->OnAnswer();

	// The dialog is deleted after execution of OnAnswer
	// Note: The answer of the dialog is not deleted.
	delete (*it);
	currentDialogs.erase(it);
}

void KDp2p_DialogManager::ProcessQuestion(KDp2p_Message *message)
{
	int questionId = message->GetInt();

	
	map <int, KDp2p_DialogFactory *>::iterator it = factories.find(questionId);
	if (it == factories.end())
	{
		char buf[5];
		memcpy(buf, &questionId, 4);
		buf[4] = 0;
		KD_LogFile::printf2("Could not find the factory for question %s\n", buf);
		return;
	}
	KDp2p_DialogFactory *dialogFactory = it->second;
	KDp2p_Dialog *dialog = dialogFactory->NewDialog(engine, message);
	dialog->answer->SetAddress(*(dialog->question->GetAddress()));
	dialog->OnQuestion(dialogFactory);
	delete dialog;
}

void KDp2p_DialogManager::ProcessTimeOut()
{
	int time = SDL_GetTicks();

	deque<KDp2p_Dialog *>::iterator it = currentDialogs.begin();

	while (it != currentDialogs.end())
	{
		if (time > (*it)->GetTimeToDiscard())
		{
				delete (*it);
				currentDialogs.erase(it);
		}
		it++;
	}
}

bool KDp2p_DialogManager::RemoveQuestion(KDp2p_Dialog *dialog)
{
	deque<KDp2p_Dialog *>::iterator it = currentDialogs.begin();

	while (it != currentDialogs.end())
	{
		if (dialog == (*it))
			break;

		it++;
	}

	if (it == currentDialogs.end())
		return false;

	delete (*it);
	currentDialogs.erase(it);
	return true;
}

void KDp2p_DialogManager::RemoveQuestionsByType(char idChar[5])
{
	unsigned int id = (idChar[0]<<24) + (idChar[1]<<16) + (idChar[2]<<8) + idChar[3];
	RemoveQuestionsByType(id);
}

void KDp2p_DialogManager::RemoveQuestionsByType(int id)
{
	deque<KDp2p_Dialog *>::iterator it = currentDialogs.begin();

	while (it != currentDialogs.end())
	{
		if (id == (*it)->GetQuestionId())
		{
			delete (*it);
			currentDialogs.erase(it);
		}
		else
			it++;
	}
}

void KDp2p_DialogManager::AddFactory(int id, KDp2p_DialogFactory *factory)
{
	factories[id] = factory;
}

void KDp2p_DialogManager::AddFactory(char idChar[5], KDp2p_DialogFactory *factory)
{
	unsigned int id = (idChar[0]<<24) + (idChar[1]<<16) + (idChar[2]<<8) + idChar[3];
	AddFactory(id, factory);
}

void KDp2p_DialogManager::RemoveFactory(int id)
{
	factories.erase(id);
}

void KDp2p_DialogManager::RemoveFactory(char idChar[5])
{
	unsigned int id = (idChar[0]<<24) + (idChar[1]<<16) + (idChar[2]<<8) + idChar[3];
	RemoveFactory(id);
}
