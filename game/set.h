#ifndef SET_H
#define SET_H

#define KD_E_CANTMOVE      -31
#define KD_E_ADDIMPOSSIBLE -32

#include "hand.h"
#include "parameter.h"
#include "row.h"

class KD_Set
{ protected:
//   char* content;
   KD_Hand*       hand;
   KD_Parameters* param;
   KD_Row**       field;
  
//   int height_pixel;
   short height;
   short width;
   short pos; /* current horizontal position */

  public:
   KD_Set();
   KD_Set (int Width, int Height, int max_in_hand, KD_Parameters* Param);
  ~KD_Set();
  
   signed AddLineAtTop (KD_Gem** Gems);
   signed TestClash();
   signed Update();
   signed TakeGems ();
   signed DropGems ();
   signed RemoveGem (KD_Gem* gem, int row);
   signed MoveRight();
   signed MoveLeft();
   
    /* drawing on screen */
    KD_Gem* GetFirstGem();
    KD_Gem* GetNextGem();   
    
  private:
    short enum_row;
};

#endif
