#ifndef HighScoresController_H
#define HighScoresController_H

#include "Controller.h"
#include "../names.h"
#include "../game/highscores.h"
#include "../interf/TitleController.h"
#include "../video/font.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"

class KD_Background;
#ifndef NO_MUSIC  
class KD_Music;
#endif

#define KD_HSC_NB_SPR  1
#define KD_HSC_NB_SPRI 1
#define KD_HSC_NB_FONT 2
#define KD_HSC_NB_IMG (KD_NB_CHAR)

class KD_HighScoresController: public KD_Controller
{ protected:
   KD_Background*     back;
   KD_SpriteInstance* spri[KD_HSC_NB_SPRI];
   KD_Sprite          spr[KD_HSC_NB_SPR];
   KD_Font*           font[KD_HSC_NB_FONT];
   KD_Image*          img[KD_HSC_NB_IMG];
#ifndef NO_MUSIC
   KD_Music*          music;
#endif
   unsigned           nb_anim_letters;
   float*             X_L;
   float*             Y_L;
   char*              C_L;
   float*             R_L;
   float*             A_L;
   unsigned long      first_tick;
  
     void    DisplayBackground();
     void    DisplayChars();
     void    DisplayFaces();
     void    DisplayTexts();
  
public:
      static KD_HighScoreTable** hst;
             KD_HighScoresController();
	virtual ~KD_HighScoresController();

	virtual bool init();
	virtual bool processEvent(int value);
	virtual bool display();
	virtual bool quit();
};

#endif
