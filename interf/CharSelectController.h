#ifndef CharSelectController_H
#define CharSelectController_H

#include "Controller.h"
#include "../names.h"
#include "../video/font.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"

class KD_Background;
class KD_BouncingText;
class KD_MessageText;

#define KD_CSC_NB_SPR  1
#define KD_CSC_NB_SPRI 1
#define KD_CSC_NB_FONT 2
#define KD_CSC_NB_IMG (KD_NB_CHAR* 2)

class KD_CharSelectController: public KD_Controller
{ protected:
   KD_Background*     back;
   KD_BouncingText*   Title;  
   KD_MessageText*    Name1;
   KD_SpriteInstance* spri[KD_CSC_NB_SPRI];
   KD_Sprite*         spr;
   KD_Font*           font[KD_CSC_NB_FONT];
   KD_Image*          img[KD_CSC_NB_IMG];
   unsigned long      first_tick;
   short              sel_char;    /* tricky: (10+ (10- sel_char)% 10) is the real selected char */
                                   /* use the GET_SEL_CHAR macro to get the real value */
   float              angle;
  
     void    DisplayBackground();
     void    DisplayChars();
  
public:
             KD_CharSelectController();
 	virtual ~KD_CharSelectController();

	virtual bool init();
	virtual bool processEvent(int value);
	virtual bool display();
	virtual bool quit();
};

#endif
