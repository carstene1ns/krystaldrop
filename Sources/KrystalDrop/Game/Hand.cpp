#include <assert.h>
#include <stdio.h>
#include <memory.h>

#include "Hand.h"
#include "../Video/Gem.h"

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
  { delete[] gems;
    gems= NULL;
  }
  gem_max= 0; 
  gem_cur= 0; 
}

/* Encapsulating */
short KD_Hand::GetType()
{ assert (gems);

  if (gem_cur== 0) return KD_E_HANDEMPTY;
  return gems[0]->GetType();  
}


short KD_Hand::GetNbGems()
{ return gem_cur; 
}


short KD_Hand::GetSpaceLeft()
{ return gem_max- gem_cur;
}
    

/* I/O */
short KD_Hand::CanTake (short type)
{ if (gem_cur== 0) return 0; /* can always take if the hand is empty */
  short type_hand= gems[0]->GetType();
  return (type_hand!= type);
}


signed KD_Hand::TakeGems (KD_Gem** src, short count)
{ assert (gems);

  if (gem_cur+ count> gem_max) return KD_E_HANDFULL;
  memcpy (gems+ gem_cur, src, sizeof(KD_Gem*)* count);
  gem_cur+= count;
  
  return 0;
}


signed KD_Hand::DropGems (KD_Gem** dest)
{ assert (gems);
  short nb= GetNbGems();

  if (gem_cur== 0) return KD_E_HANDEMPTY;
  memcpy (dest, gems, sizeof(KD_Gem*)* nb);
  gem_cur= 0;
  
  return nb;
}


signed KD_Hand::PartialDropGems (KD_Gem** dest, short count)
{ assert (gems);
  short nb= GetNbGems();

  if (gem_cur< count) return KD_E_HANDNOTENOUGH;
/* not tested */  
  memcpy (dest, gems+ count, sizeof(KD_Gem*)* nb);
  gem_cur-= count;
  
  return count;
}


void KD_Hand::Display(int XC, int YC)
{ if (gem_cur== 0) return;

  switch (gem_cur)
  { case 2: assert (gems[1]); assert (gems[0]);
            gems[1]->x= XC+6;   gems[0]->x= XC-6;
            gems[1]->y= YC;     gems[0]->y= YC;
            gems[1]->Display(gems[1]->x, gems[1]->y);
            gems[0]->Display(gems[0]->x, gems[0]->y);
            break;
           
    case 1: assert (gems[0]);
            gems[0]->x= XC; gems[0]->y= YC;
            gems[0]->Display(gems[0]->x, gems[0]->y);
            break;

   default: assert (gems[2]); assert (gems[1]); assert (gems[0]);
            gems[2]->x= XC;   gems[1]->x= XC+ 7; gems[0]->x= XC- 7;
            gems[2]->y= YC- 6;gems[1]->y= YC+ 5; gems[0]->y= YC+ 5;
            gems[2]->Display(gems[2]->x, gems[2]->y);
            gems[1]->Display(gems[1]->x, gems[1]->y);
            gems[0]->Display(gems[0]->x, gems[0]->y);
            break;
  }
}

#ifdef DEBUG
void KD_Hand::Dump()
{ short index;
  printf ("Dumping hand: gem_cur= %d ", gem_cur);
  for (index= 0; index< gem_cur; index++)
    printf (" %p", gems[index]);
  printf (" **\n");
}

short KD_Hand::SearchGem (KD_Gem* gem)
{ for (short index= 0; index< gem_cur; index++)
    if (gems[index]== gem) return index;
  return -1;
}
#endif
