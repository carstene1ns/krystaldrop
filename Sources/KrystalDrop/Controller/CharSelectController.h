#ifndef CharSelectController_H
#define CharSelectController_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"
#include "../Names.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"

class KD_Background;
class KD_BouncingText;
class KD_MessageText;
class KD_Music;

#define KD_CSC_NB_SPR  1
#define KD_CSC_NB_SPRI 1
#define KD_CSC_NB_FONT 2
#define KD_CSC_NB_IMG (KD_NB_CHAR* 2)

class KD_CharSelectController: public KD_Controller, KD_ResourceSet
{ protected:
   KD_BouncingText*   Title;  
   KD_MessageText*    Name1;
   KD_SpriteInstance* spri[KD_CSC_NB_SPRI];
   KD_Sprite*         spr;
   KD_Font*           font[KD_CSC_NB_FONT];
   KD_Image*          img[KD_CSC_NB_IMG];
#ifndef NO_MUSIC
   KD_Music *music;
#endif
   unsigned long      first_tick;
   short              sel_char;    /* tricky: (10+ (10- sel_char)% 10) is the real selected char */
                                   /* use the GET_SEL_CHAR macro to get the real value */
   float              angle;
  
     void    DisplayBackground();
     void    DisplayChars();
  
public:
             KD_CharSelectController();
 	virtual ~KD_CharSelectController();

	virtual bool Init();
	virtual bool ProcessEvent(int value);
	virtual bool Display();
	virtual bool Process();
	virtual bool Quit();
	virtual bool OnEnable();
	virtual bool OnDisable();
};

#endif
