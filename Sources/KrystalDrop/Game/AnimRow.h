#ifndef ANIM_ROW_H
#define ANIM_ROW_H

#include "Row.h"
#include "../Video/Gem.h"

class KD_GenericSet;
class KD_Memo;

/* these defines (bitfield) can be returned by KD_AnimatedRow::Update() */
/*#define KD_AR_UPDATE_NOTHING 0
#define KD_AR_UPDATE_CLASH   1*/

class KD_AnimatedRow: public KD_Row
{ protected:
   short is_taking_gem;  /* if param->IsTakeHand() && is_taking_gem, then it's this
                            row whose gems are begin taken. */
   KD_Hand* hand;
   KD_Memo* set_memo;    /* set->memo */
   KD_Memo* remove_memo; /* which gems we should remove after the update */    
  
   void UpdateBlocks (unsigned multiplier); /* multiplier is used to catch up when a lag has occured */
  
  public:
   KD_AnimatedRow::KD_AnimatedRow (short Height_In_Gems, short x_Offset, 
                                   KD_Hand* Hand, KD_Parameters* Param, KD_Memo* Memo);
  ~KD_AnimatedRow();
  
#ifdef DEBUG_SANITY_CHECK
  void SanityCheck();
#endif
 
  /*static signed CanClash (short type1, short type2);*/ //moved

  signed IsLineDown(); /* is there a gem going down as part of a line down ? */
  signed IsUpFinished(); /* is there some blocks still going up ? */
  
  /* movement */ 
    void Update();
  signed TakeFromBottom();
  signed DropAtBottom();
  signed HaveGemsToRemove();
  signed RemoveGemsInFirstBlock();
  
   void  Display();  
 
  friend class KD_GenericSet; 
};

#endif
