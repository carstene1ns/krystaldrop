#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "row.h"
#include "set.h"


#ifdef DEBUG
#include <stdio.h>
#endif
class KD_Gems;

KD_Row::KD_Row()
{ content= NULL;
  hand= NULL;
  set= NULL;
  height_in_gem= 0;
}


KD_Row::KD_Row (short Height_In_Gems, KD_Hand* Hand)
{
  assert (Height_In_Gems);
  height_in_gem= Height_In_Gems;
  
  assert (Hand);
  hand= Hand;
  
  set= NULL; // must be set later by SetSet. (yes, i know it's a stupid name)
  
  // worst case: each block is made of one gem, 1 pixel afar from another block
  // one more block for the end marker.
  // nb+ speed+ accel+ array of KD_Gem*
#define GEMBLOCK_HEADER_SIZE 6
   
  content= (char*) malloc( (Height_In_Gems+ 1)* (GEMBLOCK_HEADER_SIZE+ sizeof(KD_Gem*)));
  assert (content);
  if (content!= NULL) 
    memset (content, 0, GEMBLOCK_HEADER_SIZE);
}


KD_Row::~KD_Row()
{ 
  if (content!= NULL)
  { free (content);
    content= NULL;
  }
  
  set= NULL;
  hand= NULL;
  height_in_gem= 0;
}


void KD_Row::SetSet (KD_Set* Set)
{
  assert (Set);
  set= Set;
}



#define B_READ_NB(p_block)     ( *((short*)p_block+0) )
#define B_READ_SPEED(p_block)  ( *((short*)p_block+1) )
#define B_READ_ACCEL(p_block)  ( *((short*)p_block+2) )
#define B_READ_GEM(p_block,n)  *( (KD_Gems**) ( ( (char*)p_block+GEMBLOCK_HEADER_SIZE+n*sizeof(KD_Gems*)) ))
  
#define B_NEXT_BLOCK(p_block,n)( ((char*)p_block+GEMBLOCK_HEADER_SIZE+n*sizeof(KD_Gems*)) )
#define B_MORE_THAN_ONE_BLOCK(p_block,n) ( *((short*)B_NEXT_BLOCK(p_block,n))!= 0 )


short KD_Row::CountGems()
{ assert (content);
  if (content== NULL) return 0;
 
  short i= 0;
  char* p= content;
  char nb= 0;
  
  do 
  { nb= B_READ_NB(p);
    if (nb== 0) break;
    
    i+= nb;
    p= B_NEXT_BLOCK(p, nb);
  } while (1);
  
  return i;
}


signed KD_Row::AddAtTop (KD_Gem* Gem)
{ assert (content);

  short nb= B_READ_NB(content);
 /* if (nb== height_in_gem) 
    return KD_E_ROWFULL;
*/
printf ("%d %d\n", nb, B_MORE_THAN_ONE_BLOCK(content,nb));
char *p=(char*) B_READ_GEM(content,0);

printf ("%x \n", (void*) (B_READ_GEM(content,0)));
printf ("%x \n", B_READ_GEM(content,1));
}
