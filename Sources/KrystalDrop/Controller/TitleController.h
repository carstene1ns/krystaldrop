#ifndef TitleController_H
#define TitleController_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"
//#include "../names.h"
//#include "../video/sprite.h"
//#include "../video/spriteinstance.h"

//class KD_Background;
class KD_Font;
class KD_Sprite;
class KD_SpriteInstance;
class KD_DisplayableResource;
class KD_DisplayableInstance;
class KD_Music;

/* main title controller */
class KD_TitleController: public KD_Controller, KD_ResourceSet
{ protected:
   KD_DisplayableInstance* title[2];
   KD_DisplayableResource**        spr;
   KD_DisplayableResource*         particle;  
   KD_Font*           main_font;
   KD_Font*           big_font;
   KD_Font*			  medium_font;
   //KD_Background*     back;
   unsigned long      first_tick;
   float x_f; /* "krystal"'s x */
   float x_bounce; /* parameter for the boucing of "krystal" */
   float y_f; /* "drop"'s y */
   char state;/* "krystal"'s state */    
   char state2; /* "drop"'s state */ 
   float* Anim_Offset;
  
#ifndef NO_MUSIC
   KD_Music *music;
#endif

     void    DisplayTitle();
     void    DisplayTexts();
	 void Flash();
  
  public:
             KD_TitleController();
	virtual ~KD_TitleController();

	virtual bool Init();
	virtual bool ProcessEvent(int value);
	virtual bool Process();
	virtual bool Display();
	virtual bool Quit();
	virtual bool OnEnable();
	virtual bool OnDisable();
};

#endif
