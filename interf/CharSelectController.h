#ifndef CharSelectController_H
#define CharSelectController_H

#include "Controller.h"
#include "../interf/TitleController.h"
#include "../video/font.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"

class KD_Music;

#define KD_CSC_NB_SPR  1
#define KD_CSC_NB_SPRI 1
#define KD_CSC_NB_FONT 1

class KD_CharSelectController: public KD_Controller
{ protected:
   KD_SpriteInstance* spri[KD_CSC_NB_SPRI];
   KD_Sprite          spr[KD_CSC_NB_SPR];
   KD_Font*           font[KD_CSC_NB_FONT];
   KD_Music* music;    
   unsigned long      first_tick;
  
     void    DisplayBackground();
     void    DisplayTexts();
  
    float*    X_S; /* pointers to TitleController's variables */
    float*    Y_S;
  
public:
             KD_CharSelectController();
	virtual ~KD_CharSelectController();

	virtual bool init();
	virtual bool processEvent(int value);
	virtual bool display();
	virtual bool quit();
};

#endif
