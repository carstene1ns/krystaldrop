#include "gem.h"

KD_Gem::KD_Gem (KD_Sprite* spr, short Type): KD_SpriteInstance(spr) 
{ gem_type= Type;
  status= 0;
}

void KD_Gem::SetNeedClashTest()
{ status|= KD_S_CHECKCLASH;
}

void KD_Gem::ClearNeedClashTest()
{ status&= ~KD_S_CHECKCLASH;
}

signed KD_Gem::NeedClashTest()
{ return (status & KD_S_CHECKCLASH);
}

void KD_Gem::SetVisited()
{ status|= KD_S_VISITED;
}

void KD_Gem::ClearVisited()
{ status&= KD_S_VISITED;
}

signed KD_Gem::HasBeenVisited()
{ return (status & KD_S_VISITED); 
}
