/* hand.h

   A handful of gems
*/

#ifndef HAND_H
#define HAND_H

#define KD_E_HANDFULL  -10
#define KD_E_HANDEMPTY -11

class KD_Gem
{ public:
   short type;
   short GetType() { return type; };
}; /* temporaire */


class KD_Hand
{ private:
   KD_Gem** gems;
   short gem_max;
   short gem_cur;
   
  public:
    KD_Hand(short max_in_hand);
   ~KD_Hand(); 

   /* do not check if the types match */
    signed TakeGems (KD_Gem* src, short count);
    signed DropGems (KD_Gem* dest);
    short    GetType();
    short    GetNbGems();
};

#endif
