#ifndef GEM_H
#define GEM_H

#include "spriteinstance.h"

#define KD_S_CHECKCLASH 1

class KD_Gem: public KD_SpriteInstance
{ public:
   short gem_type;
   short status;
   short GetType() { return gem_type; };
   
             KD_Gem (KD_Sprite *spr, short Type);
/*	virtual ~KD_Gem();*/
        void SetClashTest();
        void ClearClashTest();
      signed HaveClashed();
};

#endif