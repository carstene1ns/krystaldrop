#ifndef GEM_H
#define GEM_H

#include "../../KDpp/Video/SpriteInstance.h"
#include "../Names.h"

#define KD_S_CHECKCLASH 1  /* do we want to check a clash on this gem ? */
#define KD_S_VISITED    2  /* used during the recursive clash test      */
#define KD_S_G_REMOVING 4  /* the gem is in the 'bursting' state        */
#define KD_S_G_TOREMOVE 16 /* the very last moment of the gem           */

/* the different kinds of gem */
#define KD_GEM_N_RED      0 /* normal gems */
#define KD_GEM_N_BLUE     1
#define KD_GEM_N_GREEN    2
#define KD_GEM_N_YELLOW   3
#define KD_GEM_NS_RED     4 /* gems with symbol bonus */
#define KD_GEM_NS_BLUE    5
#define KD_GEM_NS_GREEN   6
#define KD_GEM_NS_YELLOW  7
#define KD_GEM_BG         8 /* colorless ice gem */
#define KD_GEM_BC_RED     9 /* color ice gems */
#define KD_GEM_BC_BLUE   10
#define KD_GEM_BC_GREEN  11
#define KD_GEM_BC_YELLOW 12
#define KD_GEM_B         13 /* bubble */
#define KD_GEM_PA        14 /* rainbow gem */
#define KD_GEM_DA        15 /* rainbow jewel */       
#define KD_GEM_FL_UP     16 /* arrows */
#define KD_GEM_FL_DOWN   17
#define KD_GEM_FL_LEFT   18
#define KD_GEM_FL_RIGHT  19
#define KD_GEM_FI        20 /* flames in a bubble */
#define KD_GEM_BN_1      21 /* n-chains blocks */
#define KD_GEM_BN_2      22
#define KD_GEM_BN_3      23
#define KD_GEM_BN_4      24
#define KD_GEM_BN_5      25
#define KD_GEM_BN_6      26
#define KD_GEM_BN_7      27
#define KD_GEM_BN_8      28
#define KD_GEM_BN_9      29
#define KD_GEM_TR        30 /* tar */
#define KD_GEM_PO        31 /* power up */ 
/* fortunately, KD_GEM_NB_KINDS< 32, that will help for KD_BurstGem */

/* the animation filenames corresponding to the previous defines */
extern char* Gem_Anim_Filenames[KD_GEM_NB_KINDS]; 

class KD_Set;

class KD_Gem: public KD_SpriteInstance
{ protected: public:
   KD_Set* set;    /* a bit ugly, but we need it to remove the gem from the set */
   short gem_type; /* red, blue, green... */
   short status;   /* bitflags of KD_S_xxx */
  
   int x,y;

  public:
            KD_Gem (KD_Set* Set, KD_Sprite *spr, short Type);  
   
      short GetType(); /* return gem_type */
virtual signed CanClashWith (short Type);
  
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
       void OnFinishAnim (int animNo); /* check if the gem must now be removed from the field */
};

#endif
