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

#define KD_BS_DOWN 1
#define KD_BS_DROP 2
#define KD_BS_UP   4
#define KD_BS_TAKE 8
#define KD_BS_STATE_MASK (1+2+4+8)
#define KD_BS_EXTRA_MASK (256+512+1024+2048+4096)  /* the 'extra' field */
#define KD_BS_EXTRA_POS   8

#define ANIM_OFF_SIZE 30

#include "Hand.h"
#include "Parameter.h"

class KD_Gem;
class KD_GenericSet;
class KD_Memo;

class KD_Row
{ protected:
       short*  content;
       short   content_size;     /* content buffer size */
static short*  work_first_block; /* used by RemoveGems to build the new row*/
       short   height_in_gem;
       short   x_offset;         /* the absolute H position of the left side of the row */
KD_Parameters* param;            /* video/physical parameters */
  
    short  CountGems(); /* nb of gems in a row */
   signed  SplitLastBlockAt (short* last_block, short index);
   signed  JoinBlocks       (short* first_block); /* join two consecutives blocks */
  
  /* each block has the following header: 
    (nb of gems in the described block, y offset of the first gem (from the top),
     block speed, block state (one of KD_BS_xxx) + some y offset for animation,  
     block acceleration).
     After the header, there is Nb KD_Gem*. */
     void  SetBlockNb       (short* p, short n);
     void  SetBlockPosY     (short* p, short n);
     void  SetBlockSpeed    (short* p, short s);
     void  SetBlockState    (short* p, short s);
     void  SetBlockExtra    (short* p, short e);
     void  SetBlockAccel    (short* p, short a);
     void  SetBlockGem      (short* p, short index, KD_Gem* gem);
  
  public:
          KD_Row (short Height_In_Gems, short x_Offset, KD_Parameters* Param);
         ~KD_Row();

          short*  GetFirstBlock  ();
          short*  GetLastBlock   ();          
   static short*  GetNextBlock   (short* p);
   static short   IsLastBlock    (short* p);
   static short   GetBlockNb     (short* p);
   static short   GetBlockPosY   (short* p);
   static short   GetBlockSpeed  (short* p);
   static short   GetBlockState  (short* p);
   static short   GetBlockExtra  (short* p);
   static short   GetBlockAccel  (short* p);
   static KD_Gem*  GetBlockGem   (short* p, short index);
   static KD_Gem** GetBlockGems  (short* p);
          signed FindInFirstBlock(KD_Gem* gem); /* look for a gem in the first block */
   
   
   /* moving gems */
   signed AddAtTop       (KD_Gem* Gem); /* add a top on top of the row */
   signed TakeFromBottom (KD_Hand* hand); /* try to take gems from the bottom and put them in hand */
   signed DropAtBottom   (KD_Hand* hand); /* try to drop gems from the hand at the bottom */
   signed RemoveGemsInFirstBlock (KD_Memo* remove_memo);
   /* the big one: remove the gems stored in remove_memo from the first block of the row. */
    
/*   short  CompareGemTypes (short t1, short t2);*/
/* changed to hand.CanTake (short type); */

#ifdef DEBUG
    void PrintRow(); /* dump the memory */
#endif
};

#endif

