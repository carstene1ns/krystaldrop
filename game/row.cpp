#include <assert.h>
#include <stdlib.h>

#include "row.h"
#include "set.h"


KD_Row::KD_Row()
{ content= NULL;
  hand= NULL;
  set= NULL;
  height_in_gem= 0;
}


KD_Row::KD_Row (short Height_In_Gems, KD_Hand* Hand, KD_Set* Set)
{
  assert (Height_In_Gems);
  height_in_gem= Height_In_Gems;
  
  assert (Hand);
  hand= Hand;
  
  assert (Set);
  set= Set;
  
  // worst case: each block is made of one gem, 1 pixel afar from another block
  // one more block for the end marker.
  // nb+ dest_y (in gem)+ speed+ accel+ array of KD_Gem*
#define GEMBLOCK_HEADER_SIZE 8
   
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
  
  Set= NULL;
  Hand= NULL;
  height_in_gem= 0;
}


#define B_READ_NB(p_block)     ( *((short*)p_block+0) )
#define B_READ_DEST_Y(p_block) ( *((short*)p_block+1) )
#define B_READ_SPEED(p_block)  ( *((short*)p_block+2) )
#define B_READ_ACCEL(p_block)  ( *((short*)p_block+3) )
#define B_READ_GEM(p_block,n)  ( *((KD_Gem**) ( ((char*)p_block+GEMBLOCK_HEADER_SIZE))+n) )
#define B_NEXT_BLOCK(p_block,n)( ((char*)p_block+GEMBLOCK_HEADER_SIZE+n*sizeof(KD_Gems*)) )

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
  if (nb== height_in_gem) 
    return KD_E_ROWFULL;
    
  
}

