#include "gem.h"
#include "../game/set.h"
#include <assert.h>

KD_Gem::KD_Gem (KD_Set* Set, KD_Sprite* spr, short Type): KD_SpriteInstance(spr) 
{ assert (Set);
  set= Set;
  gem_type= Type;
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

void KD_Gem::LaunchBurstAnimation()
{ setAnim (1); 
}

void KD_Gem::onFinishAnim (int animNo)
{ assert (set);
  
  if (animNo== 1)
  { /* if we are here, that means a gem (or a group of gems) which were blowing have
       just finished their bursting animation. Now we can test another clashes. */
   // set->RemoveGem (this);
    set->MarkAsToBeRemoved (this);
  }
}