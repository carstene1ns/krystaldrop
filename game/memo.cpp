#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "memo.h"
#include "../video/gem.h"

KD_Gem* KD_Memo::GetGem (short index)
{ assert (index< GetSize());
  
  /* // debug code
  printf ("*Memo %p :", this);
  for (int i= 0; i< GetSize(); i++) printf ("%d %p\n", i, memo[i]);
  */
  
  return memo[index];
}


short KD_Memo::GetSize()
{ return memo.size();
}


void KD_Memo::Remember (KD_Gem* p_Gem)
{ assert (p_Gem);
  memo.push_back (p_Gem);
  
  /* // debug code
  printf ("*Memo %p remembers gem %p\n", this, p_Gem); 
  */
}


short KD_Memo::FindGem (KD_Gem* p_Gem)
{ short size= memo.size();
  short i;

  for (i= 0; i< size; i++)
   if (memo[i]== p_Gem)
     return i;

  return -1;
}


void KD_Memo::Forget (short index)
{ assert (memo.size()> 0);
  assert (memo.size()> (unsigned) index);
  memo.erase(memo.begin()+ index);
  
  /* // debug code
  printf ("*Memo %p forgets gem #%d (%p)\n", this, index, memo[index]);
  */
}


void KD_Memo::Forget (KD_Gem* p_Gem)
{ short index= FindGem(p_Gem); 
  
  assert (index>= 0);
  Forget (index);
}
