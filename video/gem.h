#ifndef GEM_H
#define GEM_H

#include "spriteinstance.h"

#define KD_S_CHECKCLASH 1
#define KD_S_VISITED    2
#define KD_S_G_REMOVING 4
#define KD_S_NEW        8
#define KD_S_G_TOREMOVE 16

class KD_Set;

class KD_Gem: public KD_SpriteInstance
{ public:
	int debugCount;

   KD_Set* set;  /* a bit ugly, but we need it to remove the gem from the set */
   short gem_type;
   short status;
   short GetType() { return gem_type; };
   
   signed CanClashWith (short Type);
   
             KD_Gem (KD_Set* Set, KD_Sprite *spr, short Type);
	     //virtual ~KD_Gem();
        void SetNeedClashTest();
        void ClearNeedClashTest();
      signed NeedClashTest();
   
        void SetVisited ();
        void ClearVisited();
      signed HasBeenVisited();
   
        void SetRemoving();
      signed IsRemoving(); /* ClearRemoving is useless */
   /*
        void SetNew();
        void ClearNew();
      signed IsNew();
   */
        void LaunchBurst();
        void onFinishAnim (int animNo);
};

#endif
