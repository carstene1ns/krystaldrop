#include <assert.h>
#include <stdio.h>

#ifdef WIN32
#include <memory.h>
#endif

#include "hand.h"

/* *********
    KD_Hand
   ********* */ 
KD_Hand::KD_Hand (short max_in_hand)
{
  gem_max= max_in_hand;
  gem_cur= 0;
  gems= new KD_Gem*[max_in_hand];
  assert (gems);
}


KD_Hand::~KD_Hand()
{
  if (gems)
  { delete gems;
    gems= NULL;
  }
  gem_max= 0; 
  gem_cur= 0; 
}


int KD_Hand::GetType()
{ assert (gems);

  if (gem_cur== 0) return KD_E_HANDEMPTY;
  gems[0]->GetType();  
}


int KD_Hand::GetNbGems()
{ return gem_cur; 
}


signed KD_Hand::TakeGems (KD_Gem* src, short count)
{ assert (gems);

  if (gem_cur+ count> gem_max) return KD_E_HANDFULL;
  memcpy (gems+ gem_cur, src, sizeof(KD_Gem*)* count);
  gem_cur+= count;
}


signed KD_Hand::DropGems (KD_Gem* dest)
{ assert (gems);
  short nb= GetNbGems();

  if (gem_cur== 0) return KD_E_HANDEMPTY;
  memcpy (dest, gems, sizeof(KD_Gem*)* nb);
  gem_cur= 0;
  
  return nb;
}