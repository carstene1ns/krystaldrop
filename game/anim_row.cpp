#include <assert.h>
#include <stdio.h>

#include "anim_row.h"
#include "memo.h"

KD_AnimatedRow::KD_AnimatedRow (short Height_In_Gems, short x_Offset, 
                                KD_Hand* Hand, KD_Parameters* Param, KD_Memo* Memo):
                                            KD_Row(Height_In_Gems, x_Offset, Param)
{
  is_taking_gem= 0;  
  
  assert (Hand);
  hand= Hand;
  
  assert (Memo);
  set_memo= Memo;
  
  remove_memo= new KD_Memo(); 
  assert (remove_memo);    
}


KD_AnimatedRow::~KD_AnimatedRow()
{
  if (remove_memo!= NULL) 
  { delete remove_memo;
    remove_memo= NULL;
  }
  
}


signed KD_AnimatedRow::CanClash (short type1, short type2)
{ return type1== type2; 
}


signed KD_AnimatedRow::IsLineDown()
{ return ( GetBlockState(GetFirstBlock())== KD_BS_DOWN ); }


signed KD_AnimatedRow::IsUpFinished()
{ short* p_block= GetFirstBlock();
  short state;
  
  while (GetBlockNb(p_block)!= 0)
  { state= GetBlockState(p_block);
    if (state!= 0 && state!= KD_BS_TAKE && state!= KD_BS_DOWN) return 0;
      
    p_block= GetNextBlock (p_block);
  }
  
  return 1;
}


void KD_AnimatedRow::Update()
{ UpdateBlocks();
  
  short* p= GetFirstBlock();
  signed cur_y;
  signed index;
  while (GetBlockNb(p))
  { cur_y= GetBlockPosY(p);
    for (index= 0; index< GetBlockNb(p); index++)
    { GetBlockGem(p,index)->y= cur_y;
      cur_y+= param->Get_Height_Gem_In_Pixel();
    }
    p= GetNextBlock(p);
  }    
}


void KD_AnimatedRow::UpdateBlocks()
{ assert (content);
  assert (param);
  assert (hand);
  
  short* p= content;
  short* last_block= NULL; /* last updated block */  

  while (GetBlockNb(p)!= 0)
  { /* update block pos & speed */
    short posy=  GetBlockPosY (p);
    short accel= GetBlockAccel(p);    
    short speed= GetBlockSpeed(p);
    short state= GetBlockState(p);    
    speed+= accel;
    posy+= speed;

    /* if first block + a gem down have finished, then stop the down movement */
    if (p== content && state== KD_BS_DOWN && posy>= param->Get_Offset_Field_Y_In_Pixel())
    { posy= param->Get_Offset_Field_Y_In_Pixel();
      speed= 0;
      accel= 0;
      SetBlockPosY (p,posy);
      SetBlockSpeed(p,speed);
      SetBlockAccel(p,accel);
      SetBlockState(p,0);
    }
    else
    { if (state== KD_BS_DROP || state== KD_BS_UP)
      { short y_compar= 
         (last_block== NULL? param->Get_Offset_Field_Y_In_Pixel()
                                : GetBlockPosY(last_block)+ GetBlockNb(last_block)* 
                                                     param->Get_Height_Gem_In_Pixel());
        /* collision with the preceding block, or the top of the field ? */
        if (posy<= y_compar)
        { /* should we test for a clash ? */
          if (state== KD_BS_DROP)
          /* remember the first gem of the block has clashed */
          /* set_memo will be filled in this function */
             GetBlockGem(p,0)->SetNeedClashTest();
          else 
          if (last_block!= NULL &&
              CanClash(GetBlockGem(p,0)->GetType(), 
                       GetBlockGem(last_block, GetBlockNb(last_block)-1)->GetType()) )
             GetBlockGem(p,0)->SetNeedClashTest();          

          /* now reset the state */
          SetBlockState(p,0);
          
          if (last_block== NULL)
          { /* collision with the first block=> we have hit the top of the field */
            /* -> stop the block */
            SetBlockPosY(p, y_compar); // = Offset_Field_Y           
            SetBlockAccel(p, 0);
            SetBlockSpeed(p, 0);
            goto end_update_block;
          }
          else
          { /* collision with another block */
            /* -> join the blocks */
            JoinBlocks (last_block);
            
            /* we must now point to last_block before searching for the next block */
            p= last_block;
            //last_block= NULL;  /* p must not be compared with itself */
            /* This trick can cause a graphical problem for a frame, 
               but next frame it will be corrected. */            
            goto end_update_block;
          } /* end of 'if (last_block== NULL)' */
        } /* end of 'posy<= y_compar' */
      } /* end of 'if (state== KD_BS_DROP || state== KD_BS_UP)' */

      /* if we are here then nothing special has occurred */
      /* -> update posY */
      SetBlockPosY (p, posy);
      SetBlockSpeed(p, speed);
    } 
    
    /* Is it the end of a take down ? */
    if (IsLastBlock(p) && state== KD_BS_TAKE && 
        GetBlockPosY(p) /*+ GetBlockNb(p)* param->h */ >
         param->Get_Offset_Field_Y_In_Pixel()+ param->Get_Height_Field_In_Pixel() )
    { /* grab the gems */
      assert (param->IsTakeHand());
      short res= hand->TakeGems (GetBlockGems(p), GetBlockNb(p));
      assert (!res); /* should not fail */

      /* remove the block */
      SetBlockNb(p,0);

      /* update the states */
      param->ClearTakeHand();
      SetBlockState (p,0);
      
      /* the while loop ends here: */
      break;
    }

  end_update_block:
    /* find next block */
    last_block= p;
    p= GetNextBlock(p);
  }

  return;
}


signed KD_AnimatedRow::TakeFromBottom()
{ signed status;
  
  status= KD_Row::TakeFromBottom (hand);
  if (status== 0) is_taking_gem= 1;
  return status;
}


signed KD_AnimatedRow::DropAtBottom()
{ signed status;
  
  status= KD_Row::DropAtBottom (hand);
  if (status== 0) is_taking_gem= 0;
    
  return status;
}


signed KD_AnimatedRow::RemoveGemsInFirstBlock()
{ signed status;
  
  status= KD_Row::RemoveGemsInFirstBlock (remove_memo);
  return status;
}


signed KD_AnimatedRow::HaveGemsToRemove()
{ assert (remove_memo);
  return (remove_memo->GetSize()!= 0);
}


void KD_AnimatedRow::Display(short b)
{ short* p_block= GetFirstBlock();
  short  nb= GetBlockNb(p_block);
  if (nb== 0) return;
    
  while (nb!= 0)
  { KD_Gem* gem= GetBlockGem(p_block, nb- 1);
    assert (gem);
    if (!(param->IsRemoving()))
       if (gem->IsRemoving()) assert (0);
         
       if (b== 1)
    gem->Display();
       
    nb--;
    if (nb== 0)
    { if (IsLastBlock(p_block)) return;
      p_block= GetNextBlock(p_block);
      nb= GetBlockNb(p_block);
      assert (nb);
    }
  }
}
