#include "gem.h"

KD_Gem::KD_Gem (KD_Sprite* spr, short Type): KD_SpriteInstance(spr) 
{ gem_type= Type;
}

void KD_Gem::SetClashTest()
{ status|= KD_S_CHECKCLASH;
}


void KD_Gem::ClearClashTest()
{ status&= ~KD_S_CHECKCLASH;
}


signed KD_Gem::HaveClashed()
{ return (status & KD_S_CHECKCLASH);
}
