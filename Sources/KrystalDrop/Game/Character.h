#ifndef Character_H
#define Character_H

#include "../../KDpp/Resources/ResourceSet.h"
#include "../../KDpp/Tools/FilePath.h"


class KD_Sprite;
class KD_SpriteInstance;

class KD_Character : public KD_ResourceSet
{
	/// Resource of the character Displayed in the background
	KD_Sprite *backgroundCharac;
	/// Instance of the character Displayed in the background
	KD_SpriteInstance *backgroundCharacInst;

	/// coordinates of the background sprite
	int xBackground, yBackground;

	/// Sprite of the chibi
	KD_Sprite *chibi;
	/// Sprite instance of the chibi
	KD_SpriteInstance *chibiInst;

public:
	KD_Character();
	~KD_Character();

	void Load(const KD_FilePath &filePath);
	void Unload();

	void setBackGroundPos(int xBackground, int yBackground);

	void DisplayBackground();
};

#endif
