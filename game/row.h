/* row.h

   One vertical row of gems
*/   

#ifndef ROW_H
#define ROW_H

#define KD_E_ROWFULL         -20
#define KD_E_ROWEMPTY        -21
#define KD_E_BUFOVERFLOW     -22
#define KD_E_IMPOSSIBLENOW   -23
#define KD_E_GEMINDEXINVALID -25
#define KD_E_GEMNOTFOUND     -26

#define KD_S_LINEDOWNBROKEN  -29

#include "memo.h"
#include "set.h"
#include "hand.h"
#include "parameter.h"

class KD_GenericSet;

class KD_Row
{ protected:
   short* content;
   short* content_browse;      /* used by GetFirstY and GetNextY */
   short  content_browse_rest; /* used by GetFirstY and GetNextY */
   short content_size; /* buffer size */
   short height_in_gem;
   short x_offset;       /* the absolute horizontal position of the left side of the row */
   KD_Hand* hand;
 /*  KD_GenericSet* set;*/
   KD_Memo* memo;
   KD_Parameters* param; /* gems speed parameters */
  
  protected:
  public:
   short is_gem_down; /* is a gem going down as part of a line down ? 
                         Generally, is_gem_down will be equal to param->IsLineDown()
                         but a line down can be locally broken if the player grabs
                         the gem which is pulled down */
   short is_taking_gem;  /* if param->IsTakeHand() && is_taking_gem, then it's this
                            row whose gems are begin taken. */
  
   short CountGems();
   signed SplitLastBlockAt (short* last_block, short index); /* y is not modified */
   signed JoinBlocks (short* first_block); /* join two consecutives blocks */
                                           /* update the y field */
      
  public:
    KD_Row();
    KD_Row (short Height_In_Gems, short x_Offset, KD_Hand* Hand, KD_Parameters* Param);
   ~KD_Row();
   
    void SetMemo  (KD_Memo* Memo);
    void SetParam (KD_Parameters* Param);
    short* GetFirstBlock();
    short GetFirstBlockCount();
   
    /* moving gems */
    signed AddAtTop (KD_Gem* Gem);
    signed TakeFromBottom();
    signed DropAtBottom();
    void /*signed*/ Update();
    signed RemoveGem (KD_Gem* gem); /* ## not fully tested */
    
    signed FindInFirstBlock (KD_Gem* gem);
    
    /* drawing on screen */
    KD_Gem* GetFirstGem();
    KD_Gem* GetNextGem();
    /* the order returned is not from top to bottom or from bottom to top. It involves blocks. */
    
    
#ifdef DEBUG
    void PrintRow();
#endif
};

#endif

