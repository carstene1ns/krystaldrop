#include "memo.h"
#include "stdio.h"
#include "stdlib.h"
#include <assert.h>

KD_Gem* KD_Memo::GetGem (short index)
{// return memo[index].p_gem; 
  return memo[index];
}

/*short KD_Memo::GetOffset (short index)
{ return memo[index].x;
}*/

short KD_Memo::GetSize()
{ return memo.size();
}

void KD_Memo::Remember (/*short X,*/ KD_Gem* p_Gem)
{ memo.push_back (p_Gem);
}

void KD_Memo::Forget (short index)
{ memo.erase(memo.begin()+ index);
}

void KD_Memo::Forget (KD_Gem* p_Gem)
{ short size= memo.size();
  short i;
 printf  ("forget ");
for (i= 0; i< size; i++)
{ printf ("%p ", memo[i]); }
  printf ("\n");
  for (i= 0; i< size; i++)
   if (memo[i]/*.p_gem*/== p_Gem)
   { Forget (i);
     return;
   }
  int k= 0;
   k++;
  assert (0); /* should be found */
}
