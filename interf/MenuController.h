#ifndef MenuController_H
#define MenuController_H

#include "Controller.h"

class KD_Background;
class KD_BouncingText;
class KD_Font;
class KD_MessageText;
class KD_Sprite;
class KD_SpriteInstance;

#define KD_MENU_GAME 1

class KD_MenuController: public KD_Controller
{ protected:
   KD_Background*     back;
   KD_SpriteInstance* ar_ri;
   KD_Sprite*         ar_r;
   KD_BouncingText*   Title;  
   KD_Font*           main_font;
   KD_Font*           text_font;
   KD_Font*           mini_font;
   KD_MessageText*    Description;
   static short       menu_type;
   short              pos;  
  
            void UpdateDescription();  
  
  public: 
             KD_MenuController();
            ~KD_MenuController();

	virtual bool init();
	virtual bool processEvent (int value);
	virtual bool display();
	virtual bool quit();
    
    static  void SetMenuType (short menu_type_);
};

#endif
