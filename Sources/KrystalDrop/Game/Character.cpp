#include <assert.h>

#include "Character.h"
#include "XMLCharacterParser.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"
#include "../../KDpp/Sound/Sound.h"


KD_CharacAnim::KD_CharacAnim(const KD_CharacAnim &that)
{
	animNumber = that.animNumber;
#ifndef NO_SOUND
	voice = that.voice;
#endif
	proba = that.proba;
}

KD_CharacAnim& KD_CharacAnim::operator = (const KD_CharacAnim& that)
{
	if (&that != this)
	{
		this->KD_CharacAnim::~KD_CharacAnim();
		::new (this) KD_CharacAnim(that);
	}
	return *this;
}

//*************************************************************

KD_Character::KD_Character()
{
	backgroundCharac = 0;
	backgroundCharacInst = 0;

	chibi = 0;
	chibiInst = 0;

	for (int i=0; i<KD_NB_ANIMTYPE; i++)
		probaSum[i]=0;
}

KD_Character::~KD_Character()
{
	
}

bool KD_Character::Load(const KD_FilePath &resourceFilePath, const KD_FilePath &actionFilePath)
{
	bool res;

	res = LoadResourceFile(resourceFilePath);
	if (!res)
		return false;
	
	backgroundCharac = (KD_Sprite *) GetResource("backgroundCharacter");
	chibi = (KD_Sprite *) GetResource("chibi");

	backgroundCharacInst = (KD_SpriteInstance *) backgroundCharac->createInstance();
	chibiInst = (KD_SpriteInstance *) chibi->createInstance();

	KD_XMLCharacterParser myParser(actionFilePath, this);
	return myParser.Parse();
}

bool KD_Character::Unload()
{
	if (backgroundCharacInst)
	{
		backgroundCharac->deleteInstance(backgroundCharacInst);
		backgroundCharacInst = 0;
	}
	if (chibiInst)
	{
		chibi->deleteInstance(chibiInst);
		chibiInst = 0;
	}

	ReleaseResource("backgroundCharacter");
	ReleaseResource("chibi");
	

	for (int j=0; j<KD_NB_ANIMTYPE; j++)
	{
#ifndef NO_SOUND
		for (unsigned int i=0; i<characAnims[j].size(); i++)
			if (characAnims[j][i].voice != NULL)
			{
				ReleaseResource(characAnims[j][i].voice);
			}
#endif
		characAnims[j].clear();
		probaSum[j]=0;
	}

	UnRegisterAllResources();

	return true;
}

void KD_Character::setBackGroundPos(int xBackground, int yBackground)
{
	this->xBackground = xBackground;
	this->yBackground = yBackground;
}

void KD_Character::DisplayBackground()
{
	backgroundCharacInst->Display(xBackground, yBackground);
}

void KD_Character::DisplayChibi(int x, int y)
{
	chibiInst->Display(x, y);
}

void KD_Character::setChibiAnim(int anim)
{
	chibiInst->setAnim(anim);
}

void KD_Character::TriggerCharacterAction(int actionNumber)
{
	if (characAnims[actionNumber].size() == 0)
		return;

	float proba = (float)rand()/(float)RAND_MAX*probaSum[actionNumber];
	float currentSum = 0.0f;

	unsigned int i;
	for (i= 0; i<characAnims[actionNumber].size(); i++)
	{
		currentSum += characAnims[actionNumber][i].proba;
		if (proba < currentSum)
			break;
	}

	assert(i != characAnims[actionNumber].size());

	backgroundCharacInst->setAnim(characAnims[actionNumber][i].animNumber);

#ifndef NO_SOUND
	characAnims[actionNumber][i].voice->PlaySound();
#endif
}
