/* ## (x,y) positioning is pretty lame for now */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "parameter.h"
#include "row.h"
#include "set.h"
#include "../video/gem.h"

#ifdef DEBUG
#include <stdio.h>
#endif

/* Everybody know macros are dangerous and tricky... I knew it...
   But I still lost hours debugging,
   to eventually discover that I forgot a pair of bracket. Aaaaargh ! */
#define GEM_PTR_SIZE (sizeof(KD_Gem*)/sizeof(short))
#define GEMBLOCK_HEADER_SIZE 3 // in short's
#define B_GEM_PTR(p_block,n)   ( p_block+ GEMBLOCK_HEADER_SIZE+ (n)*GEM_PTR_SIZE ) 

#define B_READ_NB(p_block)     ( *(p_block+0) )
#define B_READ_SPEED(p_block)  ( *(p_block+1) )
#define B_READ_ACCEL(p_block)  ( *(p_block+2) )
#define B_READ_GEM(p_block,n)  ( *( (KD_Gem**) B_GEM_PTR(p_block,(n)))) 
                                                              // ^- the @&!# bracket
#define B_WRITE_NB(p_block,x)    p_block[0]= x;
#define B_WRITE_SPEED(p_block,y) p_block[1]= y;
#define B_WRITE_ACCEL(p_block,z) p_block[2]= z;
#define B_WRITE_GEM(p_block,n,p) *( (KD_Gem**) (B_GEM_PTR(p_block,(n)))   )= p;

#define B_NEXT_BLOCK(p_block) ( B_GEM_PTR(p_block,B_READ_NB(p_block)) )
#define B_IS_LAST_BLOCK(p_block) ( *(B_NEXT_BLOCK(p_block))== 0 )
/* B_IS_LAST_BLOCK will fail if p_block points on an empty block */


KD_Row::KD_Row()
{ content= NULL;
  content_size= 0;
  hand= NULL;	
  set= NULL;
  param= NULL;
  height_in_gem= 0;
  is_gem_down= 0;
}


KD_Row::KD_Row (short Height_In_Gems, short x_Offset, KD_Hand* Hand, KD_Parameters* Param)
{ assert (Height_In_Gems);
  height_in_gem= Height_In_Gems;
  
  assert (Hand);
  hand= Hand;
  
  assert (Param);
  param= Param;
  
  set= NULL; // must be set later by SetSet. (yes, i know it's a stupid name)
  x_offset= x_Offset;
  
  // worst case: each block is made of one gem, 1 pixel afar from another block
  // one more block for the end marker.
  // nb+ speed+ accel+ array of KD_Gem*
   
//  content_size= (Height_In_Gems+ 1)* (GEMBLOCK_HEADER_SIZE* sizeof(short)+ sizeof(KD_Gem*));
/* leave some space for desperate situations */
  content_size= (Height_In_Gems* 2)* (GEMBLOCK_HEADER_SIZE* sizeof(short)+ sizeof(KD_Gem*));
  
  content= (short*) malloc (content_size);
  assert (content);
  if (content!= NULL)
    memset (content, 0, content_size);
    
  content_browse= NULL;
  content_browse_rest= 0;
}


KD_Row::~KD_Row()
{ 
  if (content!= NULL)
  { free (content);
    content= NULL;
    content_size= 0;
  }
  
  set= NULL;
  hand= NULL;
  param= NULL;
  height_in_gem= 0;
}


void KD_Row::SetSet (KD_Set* Set)
{
  assert (Set);
  set= Set;
}


void KD_Row::SetParam (KD_Parameters* Param)
{ assert (Param);
  param= Param;
  
  assert (param->Get_Height_Field_In_Pixel()>= 
          param->Get_Height_Gem_In_Pixel()* height_in_gem);
  /* or else we might experience some difficulties */
}



short KD_Row::CountGems()
{ assert (content);
  if (content== NULL) return 0;
 
  short i= 0;
  short* p= content;
  char nb= 0;
  
  do 
  { nb= B_READ_NB(p);
    if (nb== 0) break;
    
    i+= nb;
    p= B_NEXT_BLOCK(p);
  } while (1);
  
  return i;
}


signed KD_Row::SplitLastBlockAt (short* last_block, short index)
{ if (index== 0) return 0; // nothing to do

  /* split a block will increase the buffer usage by 3*2 bytes */
  /* check if there is an overflow */
  if ( ((long)B_NEXT_BLOCK(last_block)- (long)content)+ GEMBLOCK_HEADER_SIZE* (signed) sizeof(short) > content_size)
    return KD_E_BUFOVERFLOW; // content overflows
  
  short nb_gem_before= B_READ_NB(last_block);
  if (index>= nb_gem_before) return KD_E_GEMINDEXINVALID;

  /* memory content, in shorts */
  /* before:    nb  sp ac p1 p1 p2 p2 .. pindex pindex .. pend pend 0 */
  /* after:   index sp ac p1 p1 .. pindex-1 pindex-1 (nb-index) ?? ?? pindex pindex.. pend pend 0 */
  { short* to_be_moved= (short*) B_GEM_PTR(last_block, index);
    memmove ( to_be_moved+ GEMBLOCK_HEADER_SIZE,
              to_be_moved,
              (nb_gem_before- index)* GEM_PTR_SIZE* sizeof(short)+ 2); /* +2 for the ending 0 */
  }
        
  last_block[0]= index; // last_block is in fact no longer the last_block
  B_NEXT_BLOCK(last_block)[0]= nb_gem_before- index;
  
  return 0;
}


signed KD_Row::JoinBlocks (short* first_block)
{ assert (first_block);
  assert (!B_IS_LAST_BLOCK(first_block));
  
  short* next_block= B_NEXT_BLOCK(first_block);  
  short gems_in_first_block= B_READ_NB(first_block);
  assert (gems_in_first_block> 0);
  
  short last_y= B_READ_GEM(first_block, gems_in_first_block- 1)->y+ 
                                   param->Get_Height_Gem_In_Pixel();
  
  /* update the y coordinates of the second block to 'glue' with the first */
  for (short nb= 0; nb< B_READ_NB(next_block); nb++)
  { B_READ_GEM(next_block,nb)->y= last_y;
    last_y+= param->Get_Height_Gem_In_Pixel();
  }

  /* find the first short not used */
  short* last_data= next_block;
  while (!B_IS_LAST_BLOCK(last_data)) last_data= B_NEXT_BLOCK(last_data);
  last_data+= GEMBLOCK_HEADER_SIZE+ GEM_PTR_SIZE* B_READ_NB(last_data)+ 1; /* +1 for the final 0 */

  /* update the gem count in the first block */
  B_WRITE_NB(first_block,gems_in_first_block+ B_READ_NB(next_block));
  
  /* and now the big one: move the data within the buffer GEMBLOCK_HEADER_SIZE* sizeof short to the left */
  { short howmany= (long) last_data- (long) next_block- GEMBLOCK_HEADER_SIZE* sizeof(short);
    memmove (next_block, next_block+ GEMBLOCK_HEADER_SIZE, howmany);
  }

  return 0;
}


signed KD_Row::AddAtTop (KD_Gem* Gem)
{ assert (content);
  assert (param);
  assert (Gem);
#ifdef HEAVY_DEBUG	
printf ("---------- add at top\n");
#endif	
/* # when is it impossible to add ? */
  if (param->IsLineDown()) return KD_E_IMPOSSIBLENOW;
  /* too much problem if we've allowed this one ^_^ */

  short* first_block= content;
  short nb= B_READ_NB(first_block);
  short  is_new_block= 0;  
  
  if (nb!= 0) /* the row is not empty, we must make space in the buffer for the new gem */
  { if (CountGems()>= height_in_gem) return KD_E_ROWFULL;
  
    short* last_block= content;
    while (!B_IS_LAST_BLOCK(last_block)) last_block= B_NEXT_BLOCK(last_block);
		
	short  dest_offset= 0;
	short* to_be_moved= NULL;
	long   how_many= 0;

	/* if the first gem is being taken or dropped, we must create a new block
	   else we simply need to add a gem to the block. */
	if (B_READ_SPEED(first_block)!= 0 || B_READ_ACCEL(first_block)!= 0)
    { /* new block */
      /* note that we need IsLineDown== false here. */   
      to_be_moved= first_block;		
      dest_offset= GEM_PTR_SIZE+ GEMBLOCK_HEADER_SIZE;
	  how_many= ((long) B_NEXT_BLOCK(last_block))+ 2- /* points after on the final 0 */	
                 (long) (to_be_moved);
      is_new_block= 1;
    }
	else
	{ /* create a 4-bytes space before the first gem pointer */
      to_be_moved= (short*) B_GEM_PTR(first_block,0);		
      how_many= ((long) B_NEXT_BLOCK(last_block))+ 2- /* points after on the final 0 */
                 (long) to_be_moved;

	  dest_offset= GEM_PTR_SIZE;
      is_new_block= 0;
    }
	
    memmove ( to_be_moved+ dest_offset,
              to_be_moved,
              how_many* sizeof(short));
    /* content buffer should not overflow, content_size should have been chosen correctly */
    assert ( (long)(to_be_moved+ dest_offset+ how_many)- (long)content < content_size );
    /* # assert not tested */
	
  }    

  B_WRITE_NB(first_block, (is_new_block== 0)? nb+ 1: 1);
  B_WRITE_SPEED(first_block,param->Get_Line_Down_Speed());
  B_WRITE_ACCEL(first_block,param->Get_Line_Down_Accel());
  B_WRITE_GEM(first_block,0,Gem);
  B_READ_GEM(first_block,0)->y= 
    param->Get_Offset_Field_In_Pixel()- param->Get_Height_Gem_In_Pixel();
    
  if (nb== 0)
  { /* if the row was initially empty, don't forget to put the final 0 */
    B_WRITE_NB(B_NEXT_BLOCK(first_block),0);
  }

  /* update the bit field */
  param->SetLineDown();
  is_gem_down= 1;
#ifdef HEAVY_DEBUG  
PrintRow();
#endif  
  return 0;
}


#ifdef DEBUG
void KD_Row::PrintRow ()
{
  short* p= content;
  int i;
  printf ("*** PrintRow\n");
  while (*p!= 0)
  {  printf ("block [%d, (%d, %d)]\n", B_READ_NB(p), B_READ_SPEED(p), B_READ_ACCEL(p));
     for (i= 0; i< B_READ_NB(p); i++)
     { assert (B_READ_GEM(p,i));
       printf ("  Gem %p\n", B_READ_GEM(p,i));
     }
     p= B_NEXT_BLOCK(p);
  }
}
#endif


void /*signed*/ KD_Row::Update()
{ assert (content);
  assert (param);
  assert (hand);
  
//  short result= 0;
  
    /* ## NEED TO BE COMPLETED */
  /* to check: check clash, remove ? */
  
  short* p= content;
  short* last_block= NULL; /* last updated block */  
  short nb= B_READ_NB(p);
  short last_gem_y;

  while (nb)
  { 
    /* update block */
    short speed= B_READ_SPEED(p);
    short accel= B_READ_ACCEL(p);
    speed+= accel;

    /* if first block + line down + line down is finished, then it's a special case (indeed) */
    if (p== content && is_gem_down && 
    /* is_gem_down, and not param->IsLineDown() */
        B_READ_GEM(p,0)->y+ speed> param->Get_Offset_Field_In_Pixel()) 
    {
      /* stop the movement */
      speed= 0;
      accel= 0;
      B_WRITE_SPEED(p,speed);
      B_WRITE_ACCEL(p,accel);

      /* update each gem in the block */
      for (;nb>0;nb--)
      {
        KD_Gem* gem= B_READ_GEM(p, nb- 1);
        gem->y= param->Get_Offset_Field_In_Pixel()+ (nb- 1)* (param->Get_Height_Gem_In_Pixel());
      }
      
      /* update the bit field */
      param->ClearLineDown();
      is_gem_down= 0;
    }
    else
    { KD_Gem* gem;
      B_WRITE_SPEED(p,speed);

      if (speed< 0) /* the gems have been dropped */
      {
        /* collision with the preceding block, or the top of the field ? */
        if (B_READ_GEM(p,0)->y<= 
              (last_block== NULL? param->Get_Offset_Field_In_Pixel()
                                : B_READ_GEM(last_block,B_READ_NB(last_block)-1)->y+ 
                                                    param->Get_Height_Gem_In_Pixel())
              /* UGLY ! */
           /*|| (is_gem_down && B_READ_GEM(p,0)->y<= param->Get_Height_Gem_In_Pixel())*/
                                            )
        {
          /* remember the first gem of the block has clashed */
          B_READ_GEM(p,0)->SetClashTest();          

          if (last_block== NULL)
          { /* collision with the first block=> we have hit the top of the field */
          
            /* stop the block */
            B_WRITE_ACCEL(p, 0);
            B_WRITE_SPEED(p, 0);
            
            /* update y */
            for (;nb>0;nb--)
            { gem= B_READ_GEM(p, nb- 1);
              gem->y= param->Get_Offset_Field_In_Pixel()+ (nb- 1)* param->Get_Height_Gem_In_Pixel();
            }
            
            goto end_update_block; /* please allow me this or else it will be completely unreadable */
          }
          else
          { /* collision with another block */
          
            /* join the blocks */
#ifdef HEAVYDEBUG            
            PrintRow();
#endif            
            JoinBlocks (last_block);
#ifdef HEAVYDEBUG            
            PrintRow();
#endif            
            /* y has been updated in JoinBlocks */
            
            /* we must now point to last_block before searching for the next block */
            p= last_block;
            
            goto end_update_block;
          }
        /* end of 'if (last_block== NULL)' */
        }
        /* end of 'if (speed< 0)' */
        /* if we are here then nothing special has occurred so the update can be done as usual. */
      }

      /* update each gem in the block */
      for (;nb>0;nb--)
      {
        gem= B_READ_GEM(p, nb- 1);
        gem->y= gem->y+ speed;
      }
      
      last_gem_y= gem->y;
    }
    
    /* other special case: is it the end of a take down ? */
    if (B_IS_LAST_BLOCK(p) && param->IsTakeHand() &&
        B_READ_GEM(p,B_READ_NB(p)-1)->y> param->Get_Height_Field_In_Pixel())
    { /* grab the gems */
      short res;
      res= hand->TakeGems ((KD_Gem**) B_GEM_PTR(p, 0), B_READ_NB(p));
      assert (!res); /* should not fail */

      /* remove the block */
      B_WRITE_NB(p,0);

      /* update the bit field */
      param->ClearTakeHand();
      
      /* the while loop ends here: */
      break;
            
      /* ## the y coordinate of the gems is now incorrect */
    }

end_update_block:
  
    /* find next block */
    if (nb== 0)
    { last_block= p;
      p= B_NEXT_BLOCK(p);
      nb= B_READ_NB(p);
    }
  }

 // return result;
  return;
}


KD_Gem* KD_Row::GetFirstGem()
{ assert (content);

  content_browse= content;
  content_browse_rest= B_READ_NB(content_browse);
  if (content_browse_rest== 0) return NULL;

  return (B_READ_GEM(content_browse,content_browse_rest-1));
}


KD_Gem* KD_Row::GetNextGem()
{ assert (content_browse_rest);
  assert (content_browse);
  assert (content);
  
  content_browse_rest--;
  if (content_browse_rest== 0) 
  { if (B_IS_LAST_BLOCK(content_browse)) return NULL;
    content_browse= B_NEXT_BLOCK(content_browse);
    content_browse_rest= B_READ_NB(content_browse);
  }
  
  return B_READ_GEM(content_browse,content_browse_rest-1); 
  /* it's not ordered, but it's not important for the drawing */
}


signed KD_Row::TakeFromBottom()
{ assert (hand);
  assert (content);
  assert (param);
#ifdef HEAVY_DEBUG	
printf ("----------takefrombottom param->IsTakeHand=%d\n",param->IsTakeHand());
#endif
  /* if some gems are being grasped, then we cannot take other */
  if (param->IsTakeHand()) return KD_E_IMPOSSIBLENOW;
  
  /* hand full ? */
  short space_left_in_hand= hand->GetSpaceLeft();
  if (space_left_in_hand== 0) return KD_E_HANDFULL;
  
  short nb_in_hand= hand->GetNbGems();
  short count_from_last= 1;
  short nb_in_last_block;
  short* p= content; 
  short last_gem_type;
  short status= 0; /* return value */

  /* find last gem in row */
  if (B_READ_NB(p)== 0) return KD_E_ROWEMPTY;
  while (!B_IS_LAST_BLOCK(p)) p= B_NEXT_BLOCK(p);
  nb_in_last_block= B_READ_NB(p);
  last_gem_type= B_READ_GEM(p, nb_in_last_block- 1)->GetType();

  /* compare with the hand's content */
  if (nb_in_hand> 0 && last_gem_type!= hand->GetType()) return KD_E_HANDINCOMPATIBLE;
  
  /* if the row only contains 1 gem from a line down
       then we are breaking the line down locally */
  if (p== content && nb_in_last_block== 1 && is_gem_down)
  { is_gem_down= 0;
    status= KD_S_LINEDOWNBROKEN;
    /* # old hack, r.i.p */
    /* param->ClearLineDown(); */
  }
  
  
  /* now, find out how many consecutive gems we can take from the bottom, of type last_gem_type */
  /* this loop would be incorrect if we hadn't checked space_left_in_hand> 0. */
  while (count_from_last< space_left_in_hand && count_from_last< nb_in_last_block)
  {
    /* check if the above gem is of the same type */        
    if (B_READ_GEM(p, nb_in_last_block- count_from_last- 1)->GetType()!= last_gem_type)
      break;

    /* special case:  */
    /* if the row only contains 1 block and the first gem (at the top) is one of a line down
       then we are breaking the line down locally */
    if (p== content && nb_in_last_block== count_from_last+ 1 && is_gem_down)
    { is_gem_down= 0;
      status= KD_S_LINEDOWNBROKEN;
      /* # same old hack */
      /* param->ClearLineDown(); */
    }
  /* there, the caller (the set) must catch if all the row have is_gem_down== 0 to
     call ClearLineDown(); */

    count_from_last++;
  }
  /* count_from_last is equal to the number of gem to move */
if (status== KD_S_LINEDOWNBROKEN) printf ("broken %d %d\n", count_from_last, nb_in_last_block);  
  /* We first split the last block at count_from_last */
  SplitLastBlockAt (p, nb_in_last_block- count_from_last);

  /* and we update the last block */
  if (!B_IS_LAST_BLOCK(p)) p= B_NEXT_BLOCK(p);
  assert (B_IS_LAST_BLOCK(p)); /* or else the buffer has been corrupted somewhere */ 
  B_WRITE_SPEED(p,param->Get_Take_Hand_Speed());
  B_WRITE_ACCEL(p,param->Get_Take_Hand_Accel());

  /* update the bit field */
  param->SetTakeHand();
#ifdef HEAVY_DEBUG  
PrintRow();
#endif
  return status;
}


signed KD_Row::DropAtBottom()
{ assert (hand);
  assert (content);
  short nb_in_hand= hand->GetNbGems();

#ifdef HEAVY_DEBUG	
printf ("----------dropatbottom\n");
#endif
  if (nb_in_hand== 0) return KD_E_HANDEMPTY;

  /* find end of buffer */
  short* p= content;
  short nb_gem_in_row= 0;
  if (B_READ_NB(p)!= 0) /* if the row is not empty */
  { while (!B_IS_LAST_BLOCK(p))
    { p= B_NEXT_BLOCK(p);
      nb_gem_in_row+= B_READ_NB(p);
    }
	
    nb_gem_in_row+= B_READ_NB(p);	
    p= B_GEM_PTR(p, B_READ_NB(p)); // p points on nothing now
  }
 
  /* and check for buffer overflow */
  short pos= (long) p- (long) content;
  
  if (pos+ (short) (GEMBLOCK_HEADER_SIZE* sizeof(short)+ sizeof(KD_Gem*)) > content_size)
  {
    /* erm.. well...
       If we are here, the row must have more than height_in_gem gems, but the player
       has not yet lost because there are some animations underway. However, we cannot
       drop the gems here because the buffer would overflow.
       Either we drop a few gems until the buffer is full, or we are lazy and refuse the move
       I'm feeling lazy...
       Anyway, if such an unlikely situation occurs, the player has lost anyway :p
    */
    return KD_E_IMPOSSIBLENOW;
  }

  /* does gems overflow ? */
  if (nb_gem_in_row+ nb_in_hand> height_in_gem)
  {
    /* ## SET BIT LOSE */
    printf ("LOSE !\n");
  //  exit (1);
    
  }

  /* create a new block */ 
  B_WRITE_NB(p,nb_in_hand);
  B_WRITE_SPEED(p,param->Get_Drop_Hand_Speed());
  B_WRITE_ACCEL(p,param->Get_Drop_Hand_Accel());
  hand->DropGems ( (KD_Gem**) B_GEM_PTR(p,0));
  
  /* update the x,y position */
  for (short index= 0; index< nb_in_hand; index++)
  {
   // ## X ?
    B_READ_GEM(p,index)->x= x_offset;
    B_READ_GEM(p,index)->y= param->Get_Height_Field_In_Pixel()- ((index+1)* param->Get_Height_Gem_In_Pixel());

  }

  /* write the ending 0 */
  *((short*) (p+ GEMBLOCK_HEADER_SIZE+ nb_in_hand* GEM_PTR_SIZE))= 0;

#ifdef HEAVY_DEBUG
PrintRow();
#endif
  
  return 0;
}

signed KD_Row::RemoveGem (KD_Gem* gem)
/* ## not fully tested */
{
#ifdef HEAVY_DEBUG  
  printf ("before, looking for %p\n", gem);
  PrintRow();
#endif  
  
  short* p= content;
  short* last_data= NULL;
  short nb;
  short index;
  while (1)
  { nb= B_READ_NB(p);

    assert (nb); 
    if (nb== 0) /* should never occur */
      return KD_E_GEMNOTFOUND;
    
    /* search gem in the current block */
    for (index= 0; index< nb; index--)
      if (gem== B_READ_GEM(p, index)) goto found;
    
    /* seek the next block */
    p= B_NEXT_BLOCK(p);
  }
  
  found:
  
  /* find the final '0' */
  last_data= p;
  while (B_IS_LAST_BLOCK(last_data)) last_data= B_NEXT_BLOCK(last_data);
  last_data+= GEMBLOCK_HEADER_SIZE+ B_READ_NB(last_data)* GEM_PTR_SIZE+ 2;

  /* now the dirty stuff. We want to remove the gem from the block */
  short* src_p;
  short* dest_p;
  
  if (nb== 1)
  { /* if the block has only one gem, the block must be removed completely. */
    /* we will start from p. */
    dest_p= p;
    src_p= B_NEXT_BLOCK(p);
  }
  else
  { /* the block must be shortened, and updated. */
    dest_p= p+ GEMBLOCK_HEADER_SIZE+ index* GEM_PTR_SIZE;    
    src_p= dest_p+ GEM_PTR_SIZE; /* one more */
   
    B_WRITE_NB(p,nb- 1);
  }
  
  memmove (dest_p, src_p, (last_data- dest_p));
  
#ifdef HEAVY_DEBUG  
  printf ("after\n");
  PrintRow();
#endif  
  return 0;
}