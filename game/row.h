#ifndef ROW_H
#define ROW_H

#define KD_E_ROWFULL -20

#include "hand.h"

class KD_Set; // forward declaration

class KD_Row
{ private:
   char* content;
   short height_in_gem;
   KD_Hand* hand;
   KD_Set* set;
   
  protected:
   short CountGems();
      
  public:
    KD_Row();
    KD_Row (short Height_In_Gems, KD_Hand* Hand, KD_Set* Set);
   ~KD_Row();
   
    signed AddAtTop (KD_Gem* Gem);
    signed EmptyHandAtBottom ();
    signed FillHandFromBottom();
    signed Update();
    signed RemoveGem (KD_Gem* gem, int index);
    
#ifdef DEBUG
    void PrintRow();
#endif

};

#endif