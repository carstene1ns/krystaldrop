#include "Character.h"

#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"

KD_Character::KD_Character()
{
	backgroundCharac = 0;
	backgroundCharacInst = 0;

	chibi = 0;
	chibiInst = 0;
}

KD_Character::~KD_Character()
{

}

void KD_Character::Load(const KD_FilePath &filePath)
{
	LoadResourceFile(filePath);
	
	backgroundCharac = (KD_Sprite *) GetResource("backgroundCharacter");
	chibi = (KD_Sprite *) GetResource("chibi");

	backgroundCharacInst = (KD_SpriteInstance *) backgroundCharac->createInstance();
	chibiInst = (KD_SpriteInstance *) chibi->createInstance();
}

void KD_Character::Unload()
{
	backgroundCharac->deleteInstance(backgroundCharacInst);
	chibi->deleteInstance(chibiInst);
	backgroundCharacInst = 0;
	chibiInst = 0;

	ReleaseResource("backgroundCharacter");
	ReleaseResource("chibi");
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
