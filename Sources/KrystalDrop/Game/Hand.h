/* hand.h

   A handful of gems
*/

#ifndef HAND_H
#define HAND_H

#define KD_E_HANDFULL         -10
#define KD_E_HANDEMPTY        -11
#define KD_E_HANDINCOMPATIBLE -12
#define KD_E_HANDNOTENOUGH    -13

class KD_Gem;

class KD_Hand
{ private:
   KD_Gem** gems;
   short gem_max;
   short gem_cur;
   
  public:
         KD_Hand(short max_in_hand);
virtual ~KD_Hand(); 

    short    GetType()  ;
    short  GetNbGems()  ;
    short GetSpaceLeft(); /* gem_max- gem_cur */
    
   /* Can we take a gem of a given type, compared with what we have in hand ? */
   /* return 0 if the gems are compatible */  
virtual short CanTake (short type);
  
   /* these methods do not check if the types match */
    signed TakeGems (KD_Gem** src, short count);
    signed DropGems (KD_Gem** dest);
    signed PartialDropGems (KD_Gem** dest, short count); /* not tested, not used */  
  
   /* Display the gems in hand, if any. Centered in (XC, YC) */
     void  Display(int XC, int YC);
#ifdef DEBUG
     void Dump();
    short SearchGem (KD_Gem* gem);
#endif     
};

#endif
