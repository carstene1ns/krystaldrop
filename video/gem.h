#ifndef GEM_H
#define GEM_H

#include "spriteinstance.h"

class KD_Gem: public KD_SpriteInstance
{ public:
   short gem_type;
   short GetType() { return gem_type; };
   
             KD_Gem (KD_Sprite *spr, short Type);
/*	virtual ~KD_Gem();*/
};

#endif