#include <assert.h>
#include <stdlib.h>


#include "set.h"

#ifdef DEBUG
#include "stdio.h"
#endif

KD_GenericSet::KD_GenericSet (int Width, int Height, int max_in_hand, KD_Parameters* Param)
{ int i;

//  field= (KD_AnimatedRow**) malloc (Height* sizeof(KD_AnimatedRow*));
  field= new KD_AnimatedRow*[Height];
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
  for (i= 0; i< width; i++)
  { field[i]= new KD_AnimatedRow(Height, 
                         (i* param->Get_Width_Gem_In_Pixel())+ param->Get_Offset_Field_X_In_Pixel(),
                         hand, param, memo);
	assert (field[i]);
//    field[i]->SetMemo (memo);
  }
  
  enum_row= 0;
  pos= 0;
}


KD_GenericSet::~KD_GenericSet ()
{ if (field!= NULL)
  { int i;
	for (i= 0; i< height; i++)
	if (field[i]!= NULL) delete field[i];
  //  free (field);
    delete[] field;
	field= NULL;
	}
  
  if (hand!= NULL)
  { delete hand;
	hand= NULL;
  }
}

signed KD_GenericSet::IsLineDown()
{ signed index;
  
  for (index= 0; index< width; index++)
    if (field[index]->IsLineDown()) return 1;
  return 0;
}


signed KD_GenericSet::MoveLeft()
{ if (pos== 0) return KD_E_CANTMOVE;
  pos--;
  
  return 0;
}


signed KD_GenericSet::MoveRight()
{ if (pos== width- 1) return KD_E_CANTMOVE;
  pos++;
  
  return 0;
}


signed KD_GenericSet::TakeGems()
{ assert (field[pos]);
  signed status;
  
  status= field[pos]->TakeFromBottom();

  /* check if all the gems have is_gem_down when a line is going down */
  /* if that's so, then the line going down has completely disappeared */
  /* this is quite improbable even width> 3, or the player is moving 
     extremely fast :-) */
  /* ## piece of code not tested */
  /* ## after some test, this piece of code seems buggy :/ */
/* if (status== KD_S_LINEDOWNBROKEN)
  { signed index;

    for (index= 0; index< width; index++)
    { assert (field[index]);
   //   if (field[index]->is_gem_down== 1) break;
    }
    if (index== width)
    { // the unlikely case has occurred.
      param->ClearLineDown();
    }
    status= 0;
  }*/
  
  return status;
}


signed KD_GenericSet::DropGems()
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
    
#ifdef DEBUG
    if (status== KD_E_ROWFULL) printf ("Row full has occured in KD_GenericSet::AddLineAtTop()\n");
#endif      
    if (status== 0) 
    { at_last_one= 1;
      Gem[index]= NULL;
    }
  //  if (status!= 0 &&
   //     status!= KD_E_ROWFULL) /* this is not too serious, the player will lose soon. */
                               /* # maybe it could create a memory leak though. */
                               /* # one solution could be to write NULL in Gem
                                    when a gem has been successfully added. 
                                    When the function returns, the non-NULL 
                                    pointers have to be freed. */
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
  signed status;

  for (index= 0; index< width; index++)
  { assert(field[index]);
    if (field[index]->remove_memo->GetSize()!= 0)
    {
      status= field[index]->RemoveGemsInFirstBlock();
      param->ClearRemoving();
    }
  }

  return status;
}


void KD_GenericSet::MarkAsToBeRemoved (KD_Gem* Gem)
{ signed row;
  
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
  { p_block= field[index]->content;

    for (i= 0; i< KD_Row::GetBlockNb(p_block); i++)
      KD_Row::GetBlockGem(p_block, i)->ClearVisited();
  }
}


void KD_GenericSet::Update()
{ assert (field);
  signed index;
 // signed status;

  for (index= 0; index< width; index++)
  { assert (field[index]);
    /*status=*/ field[index]->Update();
    /* check clash bit */
    /* check line broken ended */
  }
  
  //return 0;
}


KD_Gem* KD_GenericSet::GetFirstGem()
{ KD_Gem* gem= field[0]->GetFirstGem();
  
  enum_row= 0;

  while (gem== NULL)
  { enum_row++;
    if (enum_row== width) return NULL;
    gem= field[enum_row]->GetFirstGem();
  }  
  
  return gem;
}


KD_Gem* KD_GenericSet::GetNextGem()
{ KD_Gem* gem= field[enum_row]->GetNextGem(); 

  while (gem== NULL)
  { enum_row++;
    if (enum_row== width) return NULL;
    gem= field[enum_row]->GetFirstGem();
  }
  
  return gem;
}


//KD_GenericSet::KD_Gene
//KD_GenericSet::KD_GenericSet() { };
//KD_Set::KD_Set(): KD_GenericSet() { };
KD_Set::KD_Set(int Width, int Height, int max_in_hand, KD_Parameters* Param):
        KD_GenericSet (Width, Height, max_in_hand, Param) { };

signed KD_Set::TestBurstStart ()
{ assert (memo);

  KD_Gem* p_gem;
  short* p_block;  
  short size= memo->GetSize();
  short index;
  short gem_pos;
  short row;  
  short type;
  short nb;
  short index_min= 0;
  short index_max= 0; 

  if (size== 0) return 0; /* the player has taken back the gem while other were bursting. */
//printf ("zer %d\n", size);
  for (index= 0; index< size;/* size--*/)
  { /* if the gem is not in the first block, then it should not start a burst. */
      /* Which row is being examined ? */
    p_gem= memo->GetGem (0);
    row= (p_gem->x- param->Get_Offset_Field_X_In_Pixel())/ param->Get_Width_Gem_In_Pixel();
    assert (row>=0 && row< width);
    
    /* it is possible that the very first block has to be checked for a clash
       but it must first go up, because a previous block has disappeared upward
       in the same row. In that case, we must wait to check */
    if ( (KD_Row::GetBlockSpeed(field[row]->GetFirstBlock()))!= 0 || 
         (KD_Row::GetBlockAccel(field[row]->GetFirstBlock()))!= 0 ) continue;
    
      /* is the gem in the first block ? */
    
    gem_pos= field[row]->FindInFirstBlock (p_gem);
    if (gem_pos< 0)
    { index++; continue; } /* not found -> do nothing */
    //memo->Forget (index);
    p_gem->ClearNeedClashTest();
    
    
    size--;
      
    /* now, we look upwards and downwards the gem to find similar gems. */
    p_block= field[row]->GetFirstBlock();
    type= p_gem->GetType();    
    nb= KD_Row::GetBlockNb(p_block);
    index_min= gem_pos;
    index_max= gem_pos;

    while (index_max< nb- 1 && 
           KD_AnimatedRow::CanClash (KD_Row::GetBlockGem(p_block, index_max+ 1)->GetType(), type) )
             index_max++;
      
    while (index_min> 0 &&
           KD_AnimatedRow::CanClash (KD_Row::GetBlockGem(p_block, index_min- 1)->GetType(), type) )
             index_min--;

    if (index_max-index_min< 2) /* not enough to initiate a burst */
       continue;

    /* first, note the fact that gems are about to be removed */
    param->SetRemoving();
    
    /* first, mark the gems between index_min and index_max as visited
       and begin burst animation */
    for (gem_pos= index_min; gem_pos<= index_max; gem_pos++)
    { KD_Row::GetBlockGem(p_block,gem_pos)->LaunchBurst();
      KD_Row::GetBlockGem(p_block,gem_pos)->SetVisited();
    }
    
    for (gem_pos= index_min; gem_pos<= index_max; gem_pos++)
      RecurseBurst (row, gem_pos, type);
  }

  ResetVisitedFlag();
  return 0;
}

void KD_Set::RecurseBurst (short row, short gem_pos, short type)
{ short* p_block;
  KD_Gem* p_gem;
 
  /* left */
  if (row> 0)
  { p_block= field[row-1]->GetFirstBlock();
    if (KD_Row::GetBlockSpeed(p_block)== 0 &&
        KD_Row::GetBlockAccel(p_block)== 0)
    if (KD_Row::GetBlockNb(p_block)> gem_pos)
    /* we can go left, note that KD_Row::GetBlockNb starts at 1 but not gem_pos */
    { p_gem= KD_Row::GetBlockGem(p_block, gem_pos);
      assert (p_gem);
      if (!(p_gem->HasBeenVisited()) &&
          KD_AnimatedRow::CanClash (p_gem->GetType(), type))
     { p_gem->LaunchBurst();
       p_gem->SetVisited();
       RecurseBurst (row- 1, gem_pos, type);
     }
      p_gem->SetVisited();
    }
  }
  
  /* right */
  if (row< width- 1)
  { p_block= field[row+1]->GetFirstBlock();
    if (KD_Row::GetBlockSpeed(p_block)== 0 &&
        KD_Row::GetBlockAccel(p_block)== 0)
    if (KD_Row::GetBlockNb(p_block)> gem_pos)
    { p_gem= KD_Row::GetBlockGem(p_block, gem_pos);
      assert (p_gem);
      if (!(p_gem->HasBeenVisited()) &&
          KD_AnimatedRow::CanClash (p_gem->GetType(), type))
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
        KD_AnimatedRow::CanClash (p_gem->GetType(), type))
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
        KD_AnimatedRow::CanClash (p_gem->GetType(), type))
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
  { if (KD_Row::GetBlockNb(field[index]->content)== 0) continue;
    if (!KD_Row::IsLastBlock(field[index]->content))  return 0;
    if ((KD_Row::GetBlockSpeed(field[index]->content)< 0) ||
        (KD_Row::GetBlockAccel(field[index]->content)< 0)) return 0;
  }
  
  return 1;
}