#include "memo.h"
#include "stdio.h"
#include "stdlib.h"
#include <assert.h>
#include "../video/gem.h"

KD_Gem* KD_Memo::GetGem (short index)
{ 
/* c reparti la chasse au bug... */
  assert (index< GetSize());
  
  /*for (int i=0; i< GetSize(); i++)
  {
 printf ("%p %d %p\n", this, i, memo[i]);
  }
  printf ("***");*/
  
  signed t= memo[index]->gem_type;
  memo[index]->gem_type= t;
  //printf ("\n");
  return memo[index];
}


short KD_Memo::GetSize()
{ return memo.size();
}


void KD_Memo::Remember (KD_Gem* p_Gem)
{ memo.push_back (p_Gem);
  KD_Gem* p= p_Gem;
  signed t= p->gem_type;
  p->gem_type= t;  
  
  //printf ("memo %p remembers gem %p\n", this, p_Gem);
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
{
  assert (memo.size()> 0);
    //printf ("memo %p forgets index %d : %p ", this, index, memo[index]);
  memo.erase(memo.begin()+ index);
}


void KD_Memo::Forget (KD_Gem* p_Gem)
{ short index= FindGem(p_Gem); 
  
  assert (index>= 0);
  Forget (index);
}
