#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "AnimRow.h"
#include "Hand.h"
#include "Memo.h"
#include "Parameter.h"
#include "Set.h"
#include "../Video/Gem.h"


KD_GenericSet::KD_GenericSet (int Width, int Height, int max_in_hand, KD_Parameters* Param)
{ field= new KD_AnimatedRow*[Height];
  assert (field);

  memo= new KD_Memo();
  assert (memo);
  
  assert (max_in_hand);
  hand= new KD_Hand(max_in_hand);
  assert (hand);

  assert (Param);
  param= Param;
	
  assert (Width> 0);
  width= Width;
  for (int i= 0; i< width; i++)
  { field[i]= new KD_AnimatedRow(Height, 
                                 (i* param->Get_Width_Gem_In_Pixel())+ 
                                  param->Get_Offset_Field_X_In_Pixel(),
                                 hand, param, memo);
	assert (field[i]);
  }
}


KD_GenericSet::~KD_GenericSet ()
{ if (field!= NULL)
  { int i;
	for (i= 0; i< width; i++)
	if (field[i]!= NULL) delete field[i];
    delete[] field;
	field= NULL;
  }
  
  if (hand!= NULL)
  { delete hand;
	hand= NULL;
  }  
}

#ifdef DEBUG_SANITY_CHECK
void KD_GenericSet::SanityCheck()
{ short index;
  
  for (index= 0; index< width; index++)
   field[index]->SanityCheck();

  KD_Gem* p_gem;
  for (index= 0; index< memo->GetSize(); index++)
  { p_gem= memo->GetGem(index);
    if (SearchGem(p_gem)< 0)
      if (hand->SearchGem(p_gem)< 0)
      { printf ("Bon là, c'est vraiment la cata. Gem %p not found (type= %d, x= %d)\n", p_gem, p_gem->GetType(),p_gem->x);
        for (int bonsang= 0; bonsang< width; bonsang++)
          field[bonsang]->PrintRow();
        hand->Dump();      
        assert (0);
      }
  }
}
#endif

signed KD_GenericSet::IsLineDown()
{ short index;
  
  for (index= 0; index< width; index++)
    if (field[index]->IsLineDown()) return 1;
  return 0;
}


KD_Parameters* KD_GenericSet::GetParameters()
{ return param; 
}


KD_Memo* KD_GenericSet::GetMemo()
{ return memo;
}
  

KD_Hand* KD_GenericSet::GetHand()
{ return hand;
}

short KD_GenericSet::GetMaxHeight()
{ signed index= 0;
  assert (field[index]);
  short  max= KD_Row::GetBlockNb(field[index]->GetFirstBlock());
  short  height;

  for (index= 1; index< width; index++)
  { assert (field[index]);
    height= KD_Row::GetBlockNb(field[index]->GetFirstBlock());
    if (height> max) max= height;
  }
  
  return max;
}


signed KD_GenericSet::TakeGems (short pos)
{ assert (field[pos]);
  
  signed status= field[pos]->TakeFromBottom();
  return status;
}


signed KD_GenericSet::DropGems (short pos)
{ assert (field[pos]);
  
  return field[pos]->DropAtBottom();
}


signed KD_GenericSet::AddLineAtTop (KD_Gem** Gem)
{ assert (Gem);
  assert (param);
  int index;
  int status;
  int at_last_one; /* set to one if at least one gem has been added */
  
  if (IsLineDown())
    return KD_E_ADDIMPOSSIBLE;
  
  if (param->IsRemoving())
    return KD_E_ADDIMPOSSIBLE;
  
  at_last_one= 0;
  
  for (index= 0; index< width; index++)
  { if (Gem[index]== NULL) continue;
    assert (field[index]);
    status= field[index]->AddAtTop (Gem[index]);
    
    if (status== 0) 
    { at_last_one= 1;
      param->SetCheckOverflow();
      Gem[index]= NULL;     
    }
  }

  /* update the bit field */  
  if (at_last_one) 
  { //param->SetLineDown();
    return 0;
  }
  else
    return KD_E_ROWFULL;
}


signed KD_GenericSet::RemoveGems()
{ signed index;
  signed status= 0; 
  
  for (index= 0; index< width; index++)
  { assert(field[index]);
    if (field[index]->remove_memo->GetSize()!= 0)
    { status= field[index]->RemoveGemsInFirstBlock();
      /* returns 0 */
/*      param->ClearRemoving();*/
    }
  }

/* kludge bug */
{ signed index;
  signed i;
  short* p_block;
  
  for (index= 0; index< width; index++)
  { p_block= field[index]->GetFirstBlock();

    for (i= 0; i< KD_Row::GetBlockNb(p_block); i++)
    { assert (KD_Row::GetBlockGem(p_block, i));
      if (KD_Row::GetBlockGem(p_block, i)->IsRemoving()) return 0;
    }
  }
  
  param->ClearRemoving();
}

  
  return status;
}


void KD_GenericSet::MarkAsToBeRemoved (KD_Gem* Gem)
{ signed row;
#ifdef DEBUG
  assert (SearchGem(Gem)>= 0);
#endif

  row= (Gem->x- param->Get_Offset_Field_X_In_Pixel())/ param->Get_Width_Gem_In_Pixel();
  assert (field);
  assert (field[row]);
  field[row]->remove_memo->Remember (Gem);
}


void KD_GenericSet::ResetVisitedFlag()
{ signed index;
  signed i;
  short* p_block;
  
  for (index= 0; index< width; index++)
  { p_block= field[index]->GetFirstBlock();

    for (i= 0; i< KD_Row::GetBlockNb(p_block); i++)
    { assert (KD_Row::GetBlockGem(p_block, i));
      KD_Row::GetBlockGem(p_block, i)->ClearVisited();
    }
  }
}


void KD_GenericSet::Update()
{ assert (field);
  signed index;

  for (index= 0; index< width; index++)
  { assert (field[index]);
    field[index]->Update();
  }
}


void KD_GenericSet::Display()
{ assert (field);
  signed index;

  for (index= 0; index< width; index++)
  { assert (field[index]);
    field[index]->Display();
  }
}


KD_Set::KD_Set(int Width, int Height, int max_in_hand, KD_Parameters* Param):KD_GenericSet (Width, Height, max_in_hand, Param) 
{ };

        
signed KD_Set::TestBurstStart ()
{ assert (memo);

  KD_Gem*         p_gem;
  KD_AnimatedRow* p_row;
  short* p_block;  
  short size= memo->GetSize();
  short index;
  short gem_pos;
  short row;  
  short type;
  short nb;
  short index_min= 0;
  short index_max= 0;
  short return_value= 0;

  if (size== 0) return 0; /* the player has taken back the gem while other were bursting. */

  for (index= 0; index< size/*memo->GetSize()*/; )
  { /* Which row is being examined ? */
    p_gem= memo->GetGem(index);
    row= (p_gem->x- param->Get_Offset_Field_X_In_Pixel())/ param->Get_Width_Gem_In_Pixel();

    assert (row>= 0 && row< width);
    p_row= field[row];
    assert (p_row);

    
#ifdef DEBUG
    /* what should we do if DEBUG is *not* defined ? */
    /* ## in fact, it would be safer to un-debug-ify SearchGem, and call it everytime */
    /* ## for now, it is because that a crash occurs */
    if (SearchGem(p_gem)< 0)
      if (hand->SearchGem(p_gem)>= 0)
    { /* It may occurs than we have taken back a gem stored in memo,
         that is, a gem that we wanted to check against a clash.
         We know this is the case when we find p_gem in hand.
         We just have to remove p_gem from memo then. */
      memo->Forget (p_gem);      /* ## not really tested, but should be ok ;) */
#ifdef DEBUG
      printf ("Removed a gem from memo that was in hand in TestBurstStart\n");
#endif
      continue;      
    }
    else
    {
     /* now this is more annoying, it should never occur */
     /* if debugging, we call SanityCheck if possible, 
        to dump memory and fail on an assert. */
     /* if not debugging, we do nothing and just pray 
        for nothing bad to happen in the future. */
     /* Note: a possibility could be the gem is not in the first block 
              nor in the hand but in a moving block. */
#ifdef DEBUG_SANITY_CHECK
      SanityCheck();
#endif
      memo->Forget (p_gem);
      continue; 
    }
#endif    
    /* if the block has already been found to burst, then there is no need to do it again */
    /* We must also remove the gem from the 'remember what to check' memo list, otherwise
       the gem will disappear from the field and the previous assert will fail */
    if ( (p_gem->IsRemoving())) 
    { p_gem->ClearNeedClashTest(); /*index++;*/ size--; continue; }
    
    /* A block must not be in a special state in order to be checked */
    if ( (p_row->GetBlockState(p_row->GetFirstBlock())) != 0)
    { index++; continue; }
    
    /* Is the gem in the first block ? If not, then it should not start a burst. */
    gem_pos= p_row->FindInFirstBlock (p_gem);
    if (gem_pos< 0)
    { index++; continue; } /* not found -> do nothing */

    /* found -> clear the needclash bit and remove from memo */
    p_gem->ClearNeedClashTest();
    size--;
      
    /* now, we look upwards and downwards the gem to find similar gems. */
    p_block= p_row->GetFirstBlock();
    type= p_gem->GetType();
    nb= p_row->GetBlockNb(p_block);
    index_min= gem_pos;
    index_max= gem_pos;

    while (index_max< nb- 1 && 
           p_row->GetBlockGem(p_block, index_max+ 1)->CanClashWith (type) )
             index_max++;
      
    while (index_min> 0 &&
           p_row->GetBlockGem(p_block, index_min- 1)->CanClashWith (type) )
             index_min--;

    if (index_max- index_min< 2) /* not enough to Initiate a burst */
       continue;

    /* first, note the fact that gems are about to be removed */
    param->SetRemoving();
    
    /* first, mark the gems between index_min and index_max as visited
       and begin burst animation */
    for (gem_pos= index_min; gem_pos<= index_max; gem_pos++)
    { KD_Row::GetBlockGem(p_block,gem_pos)->LaunchBurst();
      KD_Row::GetBlockGem(p_block,gem_pos)->SetVisited();
    }
    
    /* remember that a clash has occured, and the color */
    return_value= 1;
    clash_type= type;
    /* ## burst_gems.SaveGemType (type); mysterious error !! */
    
    /* and launch the recursive search */
    for (gem_pos= index_min; gem_pos<= index_max; gem_pos++)
      RecurseBurst (row, gem_pos, type);
  }

  ResetVisitedFlag();
  return return_value;
}


void KD_Set::RecurseBurst (short row, short gem_pos, short type)
{ short* p_block;
  KD_Gem* p_gem;
 
  /* left */
  if (row> 0)
  { p_block= field[row-1]->GetFirstBlock();
    if (KD_Row::GetBlockState(p_block)== 0)
    if (KD_Row::GetBlockNb(p_block)> gem_pos)
    /* we can go left, note that KD_Row::GetBlockNb starts at 1 but not gem_pos */
    { p_gem= KD_Row::GetBlockGem(p_block, gem_pos);
      assert (p_gem);
      if (!(p_gem->HasBeenVisited()) && p_gem->CanClashWith(type) )
     { p_gem->LaunchBurst();
       p_gem->SetVisited();
       RecurseBurst (row- 1, gem_pos, type);
     }
     else
       p_gem->SetVisited();
    }
  }
  
  /* right */
  if (row< width- 1)
  { p_block= field[row+1]->GetFirstBlock();
    if (KD_Row::GetBlockState(p_block)== 0)    
    if (KD_Row::GetBlockNb(p_block)> gem_pos)
    { p_gem= KD_Row::GetBlockGem(p_block, gem_pos);
      assert (p_gem);
      if (!(p_gem->HasBeenVisited()) &&
/*          KD_AnimatedRow::CanClash (p_gem->GetType(), type)) */
            p_gem->CanClashWith (type) )      
     { p_gem->LaunchBurst();
       p_gem->SetVisited();
       RecurseBurst (row+ 1, gem_pos, type);
     }
      p_gem->SetVisited();     
    }
  }
  
  /* down */
  p_block= field[row]->GetFirstBlock();
  assert (p_block);
  if (gem_pos< KD_Row::GetBlockNb(p_block)- 1)
  { p_gem= KD_Row::GetBlockGem(p_block, gem_pos+ 1);
    assert (p_gem);
    if (!(p_gem->HasBeenVisited()) &&
 /*       KD_AnimatedRow::CanClash (p_gem->GetType(), type)) */
          p_gem->CanClashWith (type) )
    { p_gem->LaunchBurst();
      p_gem->SetVisited();
      RecurseBurst (row, gem_pos+ 1, type);
    }
      p_gem->SetVisited();    
  }

  /* up */
  if (gem_pos> 0)
  { p_gem= KD_Row::GetBlockGem(p_block, gem_pos- 1);
    assert (p_gem);
    if (!(p_gem->HasBeenVisited()) &&
/*        KD_AnimatedRow::CanClash (p_gem->GetType(), type)) */
          p_gem->CanClashWith (type) )
    { p_gem->LaunchBurst();
      p_gem->SetVisited();
      RecurseBurst (row, gem_pos- 1, type);
    }
      p_gem->SetVisited();    
  }
}


signed KD_GenericSet::IsUpFinished()
{ short index;
  
  for (index= 0; index< width; index++)
  { assert (field[index]);
    if (!(field[index]->IsUpFinished())) return 0;
  }
  
  return 1;
}


#ifdef DEBUG
/* debug */
short tempo(short* p, KD_Gem* gem)
{ short pos;
  while (KD_Row::GetBlockNb(p)!= 0)
  {
    for (pos= 0; pos< KD_Row::GetBlockNb(p); pos++)
    { if (KD_Row::GetBlockGem(p,pos)== gem) return 1;
    }
    
    p= KD_Row::GetNextBlock (p);
  }
  
  return -1;
}


short KD_GenericSet::SearchGem (KD_Gem* gem)
{
  short index;
  short status;
  for (index= 0; index< width; index++)
  { status= tempo(field[index]->GetFirstBlock(), gem);
    if (status>= 0) return index;
  }
  
  return -1;
}
#endif
