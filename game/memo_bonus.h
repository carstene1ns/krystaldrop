/* memo_bonus.h

   KD_MemoBonus:
   Memorizes all the special bonuses that have not been taken into account
   (ice blocks, symbol gems...). Resetted when the bonus is applied.
   
   KD_BurstGems:
   Memorizes the type of the gems that have started to burst. Resetted after
   scanning the 
*/

#ifndef MEMOBONUS_H
#define MEMOBONUS_H

/* the value of these defines are important, the LSB is the first 
   bonus to be retrieved by GetNextSpecial. */
#define KD_BONUS_BN 1
#define KD_BONUS_NSR 2
#define KD_BONUS_NSB 4
#define KD_BONUS_NSG 8
#define KD_BONUS_NSY 16
/* BG ? */
#define KD_BONUS_PA 32
#define KD_BONUS_DA (64+32) /* DA overrides PA */
/* arrows ? */

class KD_MemoBonus
{ protected:
   unsigned short memo;
  
  public:
      KD_MemoBonus();

  /* save a particular bonus (one of the KD_BONUS_XX defines) */
         void SaveSpecial (unsigned short bonus); 
  
  /* retrieve the next bonus to apply, and delete it from memo, according
     to the order: least powerful bonus first.
     Returns 0 if memo stores nothing */
 signed short GetNextSpecial();
};


class KD_BurstGems
{ protected:
   unsigned long bit_field;
  
  public:
       KD_BurstGems();
  
  /* save the type of a gem that has clashed */
  void SaveGemType (short type);
  
  /* HasClashed (type) returns 1 if SaveGemType (type) has been called, then
     reset the #(type)th bit in the bit field. */
 short HasClashed  (short type);
};
  
#endif
   