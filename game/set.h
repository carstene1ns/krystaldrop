#ifndef SET_H
#define SET_H

#include "hand.h"
#include "memo.h"
#include "parameter.h"
#include "row.h"
#include "../video/gem.h"

#define KD_E_CANTMOVE      -31
#define KD_E_ADDIMPOSSIBLE -32

class KD_Row;
         
class KD_GenericSet
{ protected: public:
   KD_Hand*       hand;
   KD_Parameters* param;
   KD_Row**       field;
   KD_Memo*       memo;
  
   short height;
   short width;
   short pos; /* current horizontal position */

  public:
           KD_GenericSet (int Width, int Height, 
                          int max_in_hand, KD_Parameters* Param);
/*  virtual*/  ~KD_GenericSet();
  
   signed AddLineAtTop (KD_Gem** Gems);
   signed TestClash();
    void  Update();
   signed TakeGems ();
   signed DropGems ();
   signed RemoveGem (KD_Gem* gem, int row);
   signed MoveRight();
   signed MoveLeft();
   
    /* drawing on screen */
    KD_Gem* GetFirstGem();
    KD_Gem* GetNextGem();   
    
    /* Clash tests */
  virtual signed TestBurstStart ()= 0;
  virtual   void RecurseBurst   (short row, short gem, short type)= 0;
    
  private:
    short enum_row;
};


class KD_Set: public KD_GenericSet
{ public:
           KD_Set (int Width, int Height, 
                   int max_in_hand, KD_Parameters* Param);
 /* virtual */ ~KD_Set();
 
  virtual signed TestBurstStart ();
  virtual   void RecurseBurst   (short row, short gem, short type);
};

#endif
