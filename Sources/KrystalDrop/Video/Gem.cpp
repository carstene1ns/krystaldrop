#include <assert.h>
#include <stdio.h>

#include "Gem.h"
#include "../Game/Memo.h"
#include "../Game/Set.h"


KD_Gem::KD_Gem (KD_Set* Set, KD_Sprite* spr, short Type): KD_SpriteInstance(spr) 
{ assert (Set);
  
  set= Set;
  gem_type= Type;
  status= 0;
}


short KD_Gem::GetType() 
{ return gem_type; 
};


signed KD_Gem::CanClashWith (short Type)
{ return gem_type== Type;
}


void KD_Gem::SetNeedClashTest()
{ if (NeedClashTest()) return; /* no double call to Remember */
  status|= KD_S_CHECKCLASH;
  
  assert (set);
  assert (set->GetMemo());
  set->GetMemo()->Remember (this);
}


void KD_Gem::ClearNeedClashTest()
{ if (!NeedClashTest()) return; /* because we must not call Forget twice */
  status&= ~KD_S_CHECKCLASH;  
  
  assert (set);
  assert (set->GetMemo());
  set->GetMemo()->Forget (this);
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


void KD_Gem::SetRemoving()
{ status|= KD_S_G_REMOVING;
}


signed KD_Gem::IsRemoving()
{ return (status & KD_S_G_REMOVING);
}


void KD_Gem::LaunchBurst()
{ setAnim (1);   /* pof */
  SetRemoving();
}


void KD_Gem::OnFinishAnim (int animNo)
{ assert (set);
  
  if (animNo== 1)
  { /* if we are here, that means a gem (or a group of gems) which were blowing have
       just finished their bursting animation. */
    
    if (status& KD_S_G_TOREMOVE)
    {  /* should NOT happen */
      printf ("OnFinishAnim called twice for gem %p\n", this);
      assert (0);

      return;
    }

    /* we use this to check if a gem has called OnFinishAnim twice.
       It should never happen, but it can cause big problems if it happens. */
    status|= KD_S_G_TOREMOVE;     

    set->MarkAsToBeRemoved (this);
  }
}
