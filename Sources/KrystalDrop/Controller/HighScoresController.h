#ifndef HighScoresController_H
#define HighScoresController_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"

#include "../Names.h"
#include "../Game/Highscores.h"
#include "TitleController.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"

class KD_Background;

#define KD_HSC_NB_SPR  1
#define KD_HSC_NB_SPRI 1
#define KD_HSC_NB_FONT 2
#define KD_HSC_NB_IMG (KD_NB_CHAR)

class KD_HighScoresController: public KD_Controller, KD_ResourceSet
{ protected:
   //KD_SpriteInstance* spri[KD_HSC_NB_SPRI];
   KD_Sprite          spr[KD_HSC_NB_SPR];
   KD_Font*           font[KD_HSC_NB_FONT];
   KD_Image*          img[KD_HSC_NB_IMG];

#ifndef NO_MUSIC
   KD_Music *music;
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

	virtual bool Init();
	virtual bool ProcessEvent(int value);
	virtual bool Process();
	virtual bool Display();
	virtual bool Quit();
	virtual bool OnEnable();
	virtual bool OnDisable();
};

#endif
