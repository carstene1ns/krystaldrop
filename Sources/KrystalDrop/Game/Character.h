#ifndef Character_H
#define Character_H

#include "../../KDpp/Resources/ResourceSet.h"
#include "../../KDpp/Tools/FilePath.h"

#include <deque>
using namespace std;

class KD_Sprite;
class KD_SpriteInstance;
class KD_Sound;

class KD_CharacAnim
{
public:
#ifndef NO_SOUND
	KD_Sound *voice;
#endif
	int animNumber;
	float proba;

	KD_CharacAnim()
	{
#ifndef NO_SOUND
		voice = NULL;
#endif
		animNumber = 0;
		proba = 0;
	}

	///	Copy constructor
	KD_CharacAnim(const KD_CharacAnim &that);

	KD_CharacAnim& operator = (const KD_CharacAnim& that);
};

#define KD_ATTACK			0
#define KD_STRONGATTACK		1
#define KD_ATTACKED			2
#define KD_DANGER			3
#define KD_WINNING			4
#define KD_LOOSING			5
#define KD_NB_ANIMTYPE		6

class KD_Character : public KD_ResourceSet
{
	friend class KD_XMLCharacterParser;

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

	deque<KD_CharacAnim> characAnims[KD_NB_ANIMTYPE];
	float probaSum[KD_NB_ANIMTYPE];

public:

	KD_Character();
	~KD_Character();

	bool Load(const KD_FilePath &resourceFilePath, const KD_FilePath &actionFilePath);
	bool Unload();

	void setBackGroundPos(int xBackground, int yBackground);

	void DisplayBackground();
	void DisplayChibi(int x, int y);
	void setChibiAnim(int anim);

	/**
		Makes the character behave in a way described by actionNumber.
		Possibloe values:
			KD_ATTACK
			KD_STRONGATTACK
			KD_ATTACKED
			KD_DANGER
			KD_WINNING
			KD_LOOSING
			KD_NB_ANIMTYPE
	*/
	void TriggerCharacterAction(int actionNumber);
};

#endif
