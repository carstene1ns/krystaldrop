#ifndef GEM_H
#define GEM_H

#include "spriteinstance.h"

#define KD_S_CHECKCLASH 1  /* do we want to check a clash on this gem ? */
#define KD_S_VISITED    2  /* used during the recursive clash test      */
#define KD_S_G_REMOVING 4  /* the gem is in the 'bursting' state        */
#define KD_S_G_TOREMOVE 16 /* the very last moment of the gem           */

class KD_Set;

class KD_Gem: public KD_SpriteInstance
{ protected:
   KD_Set* set;    /* a bit ugly, but we need it to remove the gem from the set */
   short gem_type; /* red, blue, green... */
   short status;   /* bitflags of KD_S_xxx */
  
  public:
            KD_Gem (KD_Set* Set, KD_Sprite *spr, short Type);  
   
      short GetType(); /* return gem_type */
     signed CanClashWith (short Type); /* KD_AnimatedRow with gem_type and Type as arguments */
  
  /* bitwise operations on status */
       void SetNeedClashTest();
       void ClearNeedClashTest();
     signed NeedClashTest();

       void SetVisited();
       void ClearVisited();
     signed HasBeenVisited();

       void SetRemoving();
     signed IsRemoving(); /* ClearRemoving is useless */

   /* animation */
       void LaunchBurst(); /* set the bursting animation and KD_G_REMOVING flag */
       void onFinishAnim (int animNo); /* check if the gem must now be removed from the field */
};

#endif
