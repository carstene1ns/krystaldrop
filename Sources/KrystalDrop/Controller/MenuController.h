#ifndef MenuController_H
#define MenuController_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"

class KD_Background;
class KD_BouncingText;
class KD_Font;
class KD_MessageText;
class KD_Sprite;
class KD_SpriteInstance;
class KD_Music;

#define KD_MENU_GAME 1

class KD_MenuController: public KD_Controller, KD_ResourceSet
{ protected:
   KD_SpriteInstance* ar_ri;
   int				  ar_rx;
   int				  ar_ry;
   KD_Sprite*         ar_r;
   KD_BouncingText*   Title;  
   KD_Font*           main_font;
   KD_Font*           text_font;
   KD_Font*           mini_font;
   KD_MessageText*    Description;
   static short       menu_type;
   short              pos;
   
#ifndef NO_MUSIC
   KD_Music *music;
#endif
  
            void UpdateDescription();  
  
  public: 
             KD_MenuController();
            ~KD_MenuController();

	virtual bool Init();
	virtual bool ProcessEvent (int value);
	virtual bool Display();
	virtual bool Quit();
	virtual bool OnEnable();
	virtual bool OnDisable();
	virtual bool Process();
    
    static  void SetMenuType (short menu_type_);
};

#endif
