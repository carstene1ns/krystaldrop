#ifndef GEM_H
#define GEM_H

#include "spriteinstance.h"

class KD_Gem: public KD_SpriteInstance
{ public:
   short type;
   short GetType() { return type; };
   
             KD_Gem(KD_Sprite *spr): KD_SpriteInstance(spr) { };
/*	virtual ~KD_Gem();*/
   
};

#endif