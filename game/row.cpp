#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "row.h"
#include "../util/logfile.h"
#include "../video/gem.h"

#ifdef DEBUG
#include <stdio.h>
#endif

/* Everybody know macros are dangerous and tricky... I knew it...
   But I still lost hours debugging,
   to eventually discover that I forgot a pair of bracket. Aaaaargh ! */
#define GEM_PTR_SIZE (sizeof(KD_Gem*)/sizeof(short))
#define GEMBLOCK_HEADER_SIZE 5 // in short's
#define B_GEM_PTR(p_block,n)   ( p_block+ GEMBLOCK_HEADER_SIZE+ (n)*GEM_PTR_SIZE ) 

#define B_READ_NB(p_block)     ( *(p_block+0) )
#define B_READ_POSY(p_block)   ( *(p_block+1) )
#define B_READ_SPEED(p_block)  ( *(p_block+2) )
#define B_READ_ACCEL(p_block)  ( *(p_block+3) )
#define B_READ_STATE(p_block)  ( *(p_block+4) )
#define B_READ_GEM(p_block,n)  ( *( (KD_Gem**) B_GEM_PTR(p_block,(n)))) 
                                                              // ^- the @&!# bracket
#define B_WRITE_NB(p_block,x)    p_block[0]= x;
#define B_WRITE_POSY(p_block, y) p_block[1]= y;
#define B_WRITE_SPEED(p_block,y) p_block[2]= y;
#define B_WRITE_ACCEL(p_block,z) p_block[3]= z;
#define B_WRITE_STATE(p_block,s) p_block[4]= s;
#define B_WRITE_GEM(p_block,n,p) *( (KD_Gem**) (B_GEM_PTR(p_block,(n)))   )= p;

#define B_NEXT_BLOCK(p_block) ( B_GEM_PTR(p_block,B_READ_NB(p_block)) )
#define B_IS_LAST_BLOCK(p_block) ( (*(B_NEXT_BLOCK(p_block)))== 0 )
/* B_IS_LAST_BLOCK will fail if p_block points on an empty block */


short* KD_Row::work_first_block= NULL;

KD_Row::KD_Row()
{ content= NULL;
  content_size= 0;
  param= NULL;
  height_in_gem= 0;
}


KD_Row::KD_Row (short Height_In_Gems, short x_Offset, KD_Parameters* Param)
{ assert (Height_In_Gems);
  height_in_gem= Height_In_Gems;
  
  assert (Param);
  param= Param;
  
  x_offset= x_Offset;
  
  // worst case: each block is made of one gem, 1 pixel afar from another block
  // one more block for the end marker.
  // nb+ speed+ accel+ array of KD_Gem*
  // content_size= 
  //            (Height_In_Gems+ 1)* (GEMBLOCK_HEADER_SIZE* sizeof(short)+ sizeof(KD_Gem*));
  // but we leave some space for desperate situations
  content_size= (Height_In_Gems* 2)* (GEMBLOCK_HEADER_SIZE* sizeof(short)+ sizeof(KD_Gem*));
  
  content= (short*) malloc (content_size);
  assert (content);
  if (content!= NULL) memset (content, 0, content_size);
  
  if (work_first_block== NULL)
  { work_first_block= (short*) malloc (content_size);
    assert (work_first_block);
    if (work_first_block!= NULL) memset (work_first_block, 0, content_size);    
  }
    
  content_browse= NULL;
  content_browse_rest= 0;

  assert (param->Get_Height_Field_In_Pixel()>= 
          param->Get_Height_Gem_In_Pixel()* height_in_gem);
  /* or else we might experience some difficulties */
}


KD_Row::~KD_Row()
{ 
  if (content!= NULL)
  { free (content);
    content= NULL;
    content_size= 0;
  }
  
  param= NULL;
  height_in_gem= 0;
}


short* KD_Row::GetFirstBlock()         { assert (content); return content;     }
short* KD_Row::GetNextBlock (short* p) { assert (p); return B_NEXT_BLOCK(p);   }
short  KD_Row::IsLastBlock  (short* p) { assert (p); return B_IS_LAST_BLOCK(p);}  
short  KD_Row::GetBlockNb   (short* p) { assert (p); return B_READ_NB(p);      }
short  KD_Row::GetBlockPosY (short* p) { assert (p); return B_READ_POSY(p);    }
short  KD_Row::GetBlockAccel(short* p) { assert (p); return B_READ_ACCEL(p);   }
short  KD_Row::GetBlockSpeed(short* p) { assert (p); return B_READ_SPEED(p);   }
short  KD_Row::GetBlockState(short* p) { assert (p); return B_READ_STATE(p);   }
void KD_Row::SetBlockNb   (short* p, short n){ assert (p); B_WRITE_NB(p,n);         }
void KD_Row::SetBlockPosY (short* p, short n){ assert (p); B_WRITE_POSY(p,n);       }  
void KD_Row::SetBlockAccel(short* p, short a){ assert (p); B_WRITE_ACCEL(p,a);      }
void KD_Row::SetBlockSpeed(short* p, short s){ assert (p); B_WRITE_SPEED(p,s);      }
void KD_Row::SetBlockState(short* p, short s){ assert (p); B_WRITE_STATE(p,s);      }

KD_Gem** KD_Row::GetBlockGems (short* p){ assert (p); return (KD_Gem**) (B_GEM_PTR(p,0)); }
KD_Gem* KD_Row::GetBlockGem (short* p, short index) { assert (p);
                                                      assert (index>=0);
                                                      assert (index<=B_READ_NB(p));
                                                      return B_READ_GEM(p,index); }
void KD_Row::SetBlockGem (short* p, short index, KD_Gem* gem) { assert (p);
                                                                assert (index>=0);
                                                                assert (index<=B_READ_NB(p));
                                                                B_WRITE_GEM(p,index,gem); }


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
  short* last_data= GetLastBlock();
  last_data+= GEMBLOCK_HEADER_SIZE+ GEM_PTR_SIZE* B_READ_NB(last_data)+ 1; /* +1 for the final 0 */

  /* update the gem count in the first block */
  B_WRITE_NB(first_block,gems_in_first_block+ B_READ_NB(next_block));
  
  /* and now the big one: move the data within the buffer GEMBLOCK_HEADER_SIZE* sizeof short to the left */
  { short howmany= (long) last_data- (long) next_block- GEMBLOCK_HEADER_SIZE* sizeof(short);
    memmove (next_block, next_block+ GEMBLOCK_HEADER_SIZE, howmany);
  }

  return 0;
}


short* KD_Row::GetLastBlock()
{ assert (content);
  
  if (GetBlockNb(content)== 0) return content;
  
  short* last_block= content;
  while (!(IsLastBlock(last_block))) last_block= GetNextBlock(last_block);
  
  return last_block;
}


signed KD_Row::AddAtTop (KD_Gem* Gem)
{ assert (content);
  assert (param);
  assert (Gem);
#ifdef HEAVY_DEBUG	
printf ("---------- add at top\n");
#endif	

  if (param->IsRemoving()) return KD_E_IMPOSSIBLENOW;    
//if (param->IsLineDown()) return KD_E_IMPOSSIBLENOW;    /* tested in set.cpp */

  short* first_block= content;
  short nb= B_READ_NB(first_block);
  short is_new_block= 0;  
  
  if (nb!= 0) /* the row is not empty, we must make space in the buffer for the new gem */
  { if (CountGems()>= height_in_gem) return KD_E_ROWFULL;
  
    short* last_block= GetLastBlock();
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
  B_WRITE_POSY(first_block, param->Get_Offset_Field_Y_In_Pixel()- param->Get_Height_Gem_In_Pixel());
  B_WRITE_SPEED(first_block,param->Get_Line_Down_Speed());
  B_WRITE_ACCEL(first_block,param->Get_Line_Down_Accel());
  B_WRITE_GEM(first_block,0,Gem);
  B_READ_GEM(first_block,0)->x= x_offset;
/*  B_READ_GEM(first_block,0)->y= 
    param->Get_Offset_Field_Y_In_Pixel()- param->Get_Height_Gem_In_Pixel();*/
//  B_READ_GEM(first_block,0)->SetNew(); /* may be done elsewhere but just to be sure.. */
    
  if (nb== 0)
  { /* if the row was initially empty, don't forget to put the final 0 */
    B_WRITE_NB(B_NEXT_BLOCK(first_block),0);
  }

  /* the bit field (param) will be updated in set.cpp */
  //is_gem_down= 1;
  SetBlockState(first_block, KD_BS_DOWN);
  
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
  printf ("*** PrintRow this %p\n", this);
  while (*p!= 0)
  {  printf ("block [%d, (%d, %d, %d, %d)]\n", B_READ_NB(p), B_READ_POSY(p), B_READ_SPEED(p), B_READ_ACCEL(p), B_READ_STATE(p));
     for (i= 0; i< B_READ_NB(p); i++)
     { assert (B_READ_GEM(p,i));
       printf ("  Gem %p (type %d, status %d)\n", B_READ_GEM(p,i), B_READ_GEM(p,i)->GetType(), B_READ_GEM(p,i)->status);
     }
     p= B_NEXT_BLOCK(p);
  }
}
#endif


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
  if (content_browse_rest< 0) return NULL;
  if (content_browse_rest== 0) 
  { if (B_IS_LAST_BLOCK(content_browse)) return NULL;
    content_browse= B_NEXT_BLOCK(content_browse);
    content_browse_rest= B_READ_NB(content_browse);
  }
  
  return B_READ_GEM(content_browse,content_browse_rest-1); 
  /* it's not ordered, but it's not important for the drawing */
}


signed KD_Row::TakeFromBottom (KD_Hand* hand)
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

  /* find last gem in row */
  if (B_READ_NB(p)== 0) return KD_E_ROWEMPTY;
  p= GetLastBlock();
  nb_in_last_block= B_READ_NB(p);
  last_gem_type= B_READ_GEM(p, nb_in_last_block- 1)->GetType();

  /* if the last gem is being removed, we must not take it. */
  if (GetBlockGem(p, nb_in_last_block- 1)->IsRemoving()) return KD_E_IMPOSSIBLENOW;
  
  /* compare with the hand's content */
  if (nb_in_hand> 0 &&
      CompareGemTypes(last_gem_type, hand->GetType())
      ) return KD_E_HANDINCOMPATIBLE;
    
  /* now, find out how many consecutive gems we can take from the bottom, of type last_gem_type */
  /* this loop would be incorrect if we hadn't checked space_left_in_hand> 0. */
  while (count_from_last< space_left_in_hand && count_from_last< nb_in_last_block)
  {
    /* check if the above gem is of the same type */        
    if (B_READ_GEM(p, nb_in_last_block- count_from_last- 1)->GetType()!= last_gem_type)
      break;

  /* ## there, the caller (the set) must catch if all the row have is_gem_down== 0 to
     call ClearLineDown(); */
    count_from_last++;
  }
  /* count_from_last is equal to the number of gem to move */

  /* We first split the last block at count_from_last */
  short posy= GetBlockPosY(p);
  posy+= (nb_in_last_block- count_from_last)* param->Get_Height_Gem_In_Pixel();
  SplitLastBlockAt (p, nb_in_last_block- count_from_last);

  /* and we update the last block */
  assert (B_READ_NB(p)!= 0);
  p= GetLastBlock();
  SetBlockPosY(p,posy);
  B_WRITE_SPEED(p,param->Get_Take_Hand_Speed());
  B_WRITE_ACCEL(p,param->Get_Take_Hand_Accel());
  
  /* update the bit field */
  param->SetTakeHand();
#ifdef HEAVY_DEBUG  
PrintRow();
#endif
  
  /* mark this row as being the one whose gem are being taken. */
  SetBlockState (p, KD_BS_TAKE);  
  
  /* now, perhaps some gems were marking as target for a clash test.
     Then it must not happen. */
  /* ## not tested */
  short index;
  for (index= 0; index< B_READ_NB(p); index++)
    B_READ_GEM(p,index)->ClearNeedClashTest();

  return 0;
}


signed KD_Row::DropAtBottom (KD_Hand* hand)
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
  B_WRITE_POSY(p, param->Get_Height_Field_In_Pixel()); /* ## ?? */
  B_WRITE_SPEED(p,param->Get_Drop_Hand_Speed());
  B_WRITE_ACCEL(p,param->Get_Drop_Hand_Accel());
  hand->DropGems ( (KD_Gem**) B_GEM_PTR(p,0));
  
  /* update the x position */
  for (short index= 0; index< nb_in_hand; index++)
  {
    B_READ_GEM(p,index)->x= x_offset;
    /*B_READ_GEM(p,index)->y= param->Get_Offset_Field_Y_In_Pixel()+ 
                            param->Get_Height_Field_In_Pixel()- 
                            ((nb_in_hand-index)* param->Get_Height_Gem_In_Pixel());*/
    B_READ_GEM(p,index)->ClearNew();
  }

  /* write the ending 0 */
  *((short*) (p+ GEMBLOCK_HEADER_SIZE+ nb_in_hand* GEM_PTR_SIZE))= 0;

#ifdef HEAVY_DEBUG
PrintRow();
#endif
  
  /* no matter what the previous value was... (a test would be useless) */
  //is_taking_gem= 0;
  SetBlockState (p, KD_BS_DROP);
  
  return 0;
}


signed KD_Row::RemoveGemsInFirstBlock (KD_Memo* remove_memo)
{
  short* p= content;
  short nb;
  short index;
  KD_Gem* gem;
printf ("remove\n"); 
  assert (remove_memo);
  if (remove_memo->GetSize()== 0) return 0;
PrintRow();

  short* last_data= p;
  if (B_READ_NB(last_data)!= 0)
  { while (!B_IS_LAST_BLOCK(last_data)) last_data= B_NEXT_BLOCK(last_data);
    last_data+= GEMBLOCK_HEADER_SIZE+ B_READ_NB(last_data)* GEM_PTR_SIZE;
  }
  last_data++;
  
  nb= B_READ_NB(p);
  index= 0;
  short to_remove= remove_memo->GetSize();
  short removed= to_remove;

printf ("to_remove %d\n", to_remove);  
  while (index< nb && to_remove> 0)
  { short index_memo;
    
    gem= B_READ_GEM(p, index);
    for (index_memo= 0; index_memo< to_remove; index_memo++)
     if (gem== remove_memo->GetGem(index_memo))
      { B_WRITE_GEM(p,index,NULL);
        remove_memo->Forget (gem);
        delete gem;

        to_remove--;
        break;
      }

    index++;
  }
  
  assert (to_remove== 0);

  /* now construct the new first blocks */
  short* pos_new_buf= work_first_block;
  short new_speed= B_READ_SPEED(p);
  short new_accel= B_READ_ACCEL(p);
  short new_state= 0;
  short new_posy= param->Get_Offset_Field_Y_In_Pixel();
  
  /* By default, we don't change the speed/accel of the first block. */
  /* But if the first gem disappears, then the first block must go up*/
  if (B_READ_GEM(p, 0)== NULL)
  { new_speed= param->Get_Gem_Up_Speed();
    new_accel= param->Get_Gem_Up_Accel();
    new_state= KD_BS_UP;
  }
    
  
  index= 0;
  short nb_in_block= 0;
  while (index< nb/*- removed*/)
  { 
    if (B_READ_GEM(p, index)!= NULL)
    { nb_in_block++;
      B_WRITE_GEM(pos_new_buf,nb_in_block-1,B_READ_GEM(p,index));
    }
    else
    if (nb_in_block> 0)
    { /* new block */
      if (!(pos_new_buf== work_first_block))
        B_READ_GEM(pos_new_buf,0)->ClearNew();
      B_WRITE_NB(pos_new_buf, nb_in_block);
      B_WRITE_POSY(pos_new_buf, new_posy);
      B_WRITE_SPEED(pos_new_buf, new_speed);
      B_WRITE_ACCEL(pos_new_buf, new_accel);
      SetBlockState (pos_new_buf, new_state);
      new_speed= param->Get_Gem_Up_Speed();
      new_accel= param->Get_Gem_Up_Accel();
      new_state= KD_BS_UP;
      pos_new_buf+= GEMBLOCK_HEADER_SIZE+ nb_in_block* GEM_PTR_SIZE;
      nb_in_block= 0;
    }
    
    index++;
    new_posy+= param->Get_Height_Gem_In_Pixel();
  } 
  
  /* at the end of the while loop, we must create a block for the remaining gems */
   if (nb_in_block> 0)
    { /* new block */
      B_WRITE_NB(pos_new_buf, nb_in_block);
      B_WRITE_POSY(pos_new_buf, new_posy);
      B_WRITE_SPEED(pos_new_buf, new_speed);
      B_WRITE_ACCEL(pos_new_buf, new_accel);
      SetBlockState (pos_new_buf,new_state);
      pos_new_buf+= GEMBLOCK_HEADER_SIZE+ nb_in_block* GEM_PTR_SIZE;      
    }  
    
  /* complete work_first_block */
  long how_many= (long) last_data- (long) B_NEXT_BLOCK(p);
  memcpy (pos_new_buf, B_NEXT_BLOCK(p), how_many);
  pos_new_buf= (short*) (((char*) pos_new_buf)+ how_many);
    
  /* now copy the temporary row into content */
  how_many= (long) pos_new_buf- (long) work_first_block;
  memcpy (p, work_first_block, how_many);
PrintRow();
  return 0;
}


signed KD_Row::FindInFirstBlock (KD_Gem* p_Gem)
{ short index;
  short size= B_READ_NB(content);

  for (index= 0; index< size; index++)
   if (B_READ_GEM(content, index)== p_Gem)
    return index;
     
  return -1;
}


/* should be virtual */

short KD_Row::CompareGemTypes (short t1, short t2)
/* return 0 if the gems are compatible */
{ return (t1!= t2); }
