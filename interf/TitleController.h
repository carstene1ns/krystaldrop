#ifndef TitleController_H
#define TitleController_H

#include "Controller.h"
#include "../video/spriteinstance.h"

class KD_Sprite;
class KD_Font;

/* title sprites */
class KD_Title_Krystal: public KD_SpriteInstance
{ protected:
   short state;
   short xs_c;
  
  public:
        KD_Title_Krystal (short X, short Y, KD_Sprite* spr);
   void onFinishAnim (int animNo);
};


class KD_Title_Drop: public KD_SpriteInstance
{ public:
        KD_Title_Drop (short X, short Y, KD_Sprite* spr);  
   void onFinishAnim (int animNo);
};


  
/* main title controller */
#define KD_TC_BACKGROUND_SPR 40

class KD_TitleController: public KD_Controller
{ protected:
   KD_SpriteInstance* title[3];
   KD_Sprite*         spr;
   KD_Font*           main_font;
  
   short X_S[KD_TC_BACKGROUND_SPR];
   short Y_S[KD_TC_BACKGROUND_SPR];
  
     void    InitBackgroundXY();
     void    DisplayBackground();
  
public:
             KD_TitleController();
	virtual ~KD_TitleController();

	virtual bool init();
	virtual bool processEvent(int value);
	virtual bool display();
	virtual bool quit();
};

#endif

