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

#include "hand.h"
#include "parameter.h"

class KD_Gem;
class KD_GenericSet;
class KD_Memo;

class KD_Row
{ protected:
   short* content;
   short  content_size; /* buffer size */  
   static short* work_first_block; /* used by RemoveGems */
   short  height_in_gem;
   short  x_offset;       /* the absolute horizontal position of the left side of the row */
   KD_Parameters* param; /* gems speed parameters */

    short  CountGems();
   signed  SplitLastBlockAt (short* last_block, short index);
   signed  JoinBlocks   (short* first_block); /* join two consecutives blocks */
     void  SetBlockNb   (short* p, short n);
     void  SetBlockPosY (short* p, short n);
     void  SetBlockSpeed(short* p, short s);
     void  SetBlockState(short* p, short s);
     void  SetBlockAccel(short* p, short a);
     void  SetBlockGem  (short* p, short index, KD_Gem* gem);
  
  public:
    KD_Row();
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
   static short   GetBlockAccel  (short* p);
   static KD_Gem*  GetBlockGem   (short* p, short index);
   static KD_Gem** GetBlockGems  (short* p);
          signed FindInFirstBlock(KD_Gem* gem);    
   
    /* moving gems */
   signed AddAtTop       (KD_Gem* Gem);
   signed TakeFromBottom (KD_Hand* hand);
   signed DropAtBottom   (KD_Hand* hand);
   signed RemoveGemsInFirstBlock (KD_Memo* remove_memo);
    
    /* Compare Gems. Can we take the gems of type t1 
       if we have gems of type t2 in the hands ? */
    short CompareGemTypes (short t1, short t2);

#ifdef DEBUG
    void PrintRow();
#endif
};

#endif

