#ifndef CharSelect2Controller_H
#define CharSelect2Controller_H

#include "Controller.h"
#include "../names.h"
#include "../video/font.h"
#include "../video/sprite.h"
#include "../video/spriteinstance.h"

class KD_Background;
class KD_BouncingText;
class KD_MessageText;

#define KD_CSC2_NB_SPR  1
//#define KD_CSC2_NB_SPRI 1
#define KD_CSC2_NB_FONT 2
#define KD_CSC2_NB_IMG ((KD_NB_CHAR* 2)+ 3)
/* n large pictures, n small pictures, 2 borders, 1 'vs' */

class KD_CharSelect2Controller: public KD_Controller
{ protected:
   KD_Background*     back;
   KD_BouncingText*   Title;  
   KD_MessageText*    Name1;
   KD_MessageText*    Name2;  
//   KD_SpriteInstance* spri[KD_CSC2_NB_SPRI];
//   KD_Sprite*         spr;
   KD_Font*           font[KD_CSC2_NB_FONT];
   KD_Image*          img[KD_CSC2_NB_IMG];
   float              angle[KD_NB_CHAR]; /* Y angle for small pictures */
   short              sel_char1;   /* tricky: (10+ (10- sel_char)% 10) is the real selected char */
                                   /* use the GET_SEL_CHAR macro to get the real value */
   short              sel_char2;   /* player 2 */  
   short              old_char1, old_char2; /* previously selected char */
   float              angle1, angle2; /* Y angle for large pictures */
   char               selected_char1;
   char               selected_char2;
   float              offset_y;    /* offset for final animation when leaving */
    
     void    ChangeChar (short player, signed short mod);
     void    DisplayBackground();
     void    DisplayChars();
     void    DisplayCursors();
  
public:
             KD_CharSelect2Controller();
 	virtual ~KD_CharSelect2Controller();

	virtual bool init();
	virtual bool processEvent(int value);
	virtual bool display();
	virtual bool quit();
};

#endif
