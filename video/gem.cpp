#include "gem.h"
#include "../game/set.h"
#include <assert.h>

KD_Gem::KD_Gem (KD_Set* Set, KD_Sprite* spr, short Type): KD_SpriteInstance(spr) 
{ assert (Set);
  set= Set;
  gem_type= Type;
  status= KD_S_NEW;
}

//KD_Gem::~KD_Gem() {}

signed KD_Gem::CanClashWith (short Type)
{ return KD_AnimatedRow::CanClash (gem_type, Type); 
}


void KD_Gem::SetNeedClashTest()
{ if (NeedClashTest()) return; /* no double call to Remember */
  status|= KD_S_CHECKCLASH;
  
  assert (set);
  assert (set->memo);
  set->memo->Remember (this);
}

void KD_Gem::ClearNeedClashTest()
{ if (!NeedClashTest()) return; /* because we must not call Forget here */
  status&= ~KD_S_CHECKCLASH;  
  
  assert (set);
  assert (set->memo);
  set->memo->Forget (this);
}

signed KD_Gem::NeedClashTest()
{ return (status & KD_S_CHECKCLASH);
}

void KD_Gem::SetVisited()
{ status|= KD_S_VISITED;
}

void KD_Gem::ClearVisited()
{ status&= ~KD_S_VISITED;
}

signed KD_Gem::HasBeenVisited()
{ return (status & KD_S_VISITED); 
}
/*
void KD_Gem::SetNew()
{ status|= KD_S_NEW;
}

void KD_Gem::ClearNew()
{ status&= ~KD_S_NEW;
}

signed KD_Gem::IsNew()
{ return (status & KD_S_NEW);
}*/

void KD_Gem::SetRemoving()
{ status|= KD_S_G_REMOVING;
}

signed KD_Gem::IsRemoving()
{ return (status & KD_S_G_REMOVING);
}

void KD_Gem::LaunchBurst()
{ setAnim (1); 
  SetRemoving();
}

void KD_Gem::onFinishAnim (int animNo)
{ assert (set);
  if (animNo== 1)
  { /* if we are here, that means a gem (or a group of gems) which were blowing have
       just finished their bursting animation. Now we can test another clashes. */
    if (status& KD_S_G_TOREMOVE)
    { 
      status= 1234;
      assert (0);
    }
    /* ## test purpose */ status|= KD_S_G_TOREMOVE;
    
    set->MarkAsToBeRemoved (this);
  }
}
