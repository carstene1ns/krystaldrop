#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Memo.h"
#include "../Video/Gem.h"

KD_Gem* KD_Memo::GetGem (short index)
{ assert (index< GetSize());
  
  return memo[index];
}


short KD_Memo::GetSize()
{ return memo.size();
}


void KD_Memo::Remember (KD_Gem* p_Gem)
{ assert (p_Gem);
  
  memo.push_back (p_Gem);
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
}


void KD_Memo::Forget (KD_Gem* p_Gem)
{ short index= FindGem(p_Gem); 
  
  assert (index>= 0);
  Forget (index);
}
