#ifndef CharSelect2Controller_H
#define CharSelect2Controller_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"
#include "../Names.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Video/SpriteInstance.h"

class KD_BouncingText;
class KD_MessageText;
class KD_Music;

#define KD_CSC2_NB_SPR  1
//#define KD_CSC2_NB_SPRI 1
#define KD_CSC2_NB_FONT 2
#define KD_CSC2_NB_IMG ((KD_NB_CHAR* 2)+ 3)
/* n large pictures, n small pictures, 2 borders, 1 'vs' */

class KD_CharSelect2Controller: public KD_Controller, KD_ResourceSet
{ protected:
   KD_BouncingText*   Title;  
   KD_MessageText*    Name1;
   KD_MessageText*    Name2;  
   KD_Font*           font[KD_CSC2_NB_FONT];
   KD_Image*          img[KD_CSC2_NB_IMG];
#ifndef NO_MUSIC
   KD_Music *music;
#endif
   float              angle[KD_NB_CHAR]; /* Y angle for small pictures */
   short              sel_char1;   /* tricky: (10+ (10- sel_char)% 10) is the real selected char */
                                   /* use the GET_SEL_CHAR macro to get the real value */
   short              sel_char2;   /* player 2 */  
   short              old_char1, old_char2; /* previously selected char */
   float              angle1, angle2; /* Y angle for large pictures */
   char               selected_char1;
   char               selected_char2;
   float              offset_y;    /* offset for final animation when leaving */

   int				delayedGotoTime;
   bool				enableDelayedGoto;
    
     void    ChangeChar (short player, signed short mod);
     void    DisplayBackground();
     void    DisplayChars();
     void    DisplayCursors();
  
public:
             KD_CharSelect2Controller();
 	virtual ~KD_CharSelect2Controller();

	virtual bool Init();
	virtual bool Process();
	virtual bool ProcessEvent(int value);
	virtual bool Display();
	virtual bool Quit();
	virtual bool OnEnable();
	virtual bool OnDisable();
};

#endif
