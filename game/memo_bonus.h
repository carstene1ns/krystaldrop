/* memo_bonus.h

   Memorizes all the special bonuses that have not been taken into account
   (ice blocks, symbol gems...)
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

#endif
   