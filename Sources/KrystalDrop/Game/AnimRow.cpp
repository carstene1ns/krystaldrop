#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "AnimRow.h"
#include "Memo.h"
#include "../../KDpp/Video/Display.h"

static short Anim_OffsetY[ANIM_OFF_SIZE];
#define PER_SEC 4
#define DEC 0.03
#define AMP 8.0

#define UPDATE_REFRESH_RATE 50
/* Speeds are given for 50 Hz (pos is incremented each 1/50 s) */
/* 50 Hz= 20 ms */
#define UPDATE_QUANTUM (1000/UPDATE_REFRESH_RATE)

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
  
  for (short i= 1; i<= ANIM_OFF_SIZE; i++)
    Anim_OffsetY[ANIM_OFF_SIZE- i]= (short int)
      (AMP* cos(PER_SEC* i/(2*3.14159))* exp(-DEC* i));
}


KD_AnimatedRow::~KD_AnimatedRow()
{
  if (remove_memo!= NULL) 
  { delete remove_memo;
    remove_memo= NULL;
  } 
}

#ifdef DEBUG_SANITY_CHECK
void KD_AnimatedRow::SanityCheck()
{ short* p_block= GetFirstBlock();
  short  nb= GetBlockNb(p_block);
  
/* plus search remove_gem */    
  while (nb!= 0)
  { KD_Gem* gem= GetBlockGem(p_block, nb- 1);
    assert (gem);
  
    nb--;
    if (nb== 0)
    { if (IsLastBlock(p_block)) return;
      p_block= GetNextBlock(p_block);
      nb= GetBlockNb(p_block);
      assert (nb);
    }
  }
}
#endif

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


void KD_AnimatedRow::Update ()
{ short* p= GetFirstBlock();
  signed cur_y;
  signed index;
  signed off_y;
  signed ind_off_y;
//  unsigned return_value= 0;
  
  /* # is it necessary ? */
  /*
  while (GetBlockNb(p))
  { ind_off_y= GetBlockExtra(p);
    if (ind_off_y== 0) off_y= 0;
    else { off_y= Anim_OffsetY[ind_off_y];
         }
         
    cur_y= GetBlockPosY(p)- off_y;
    for (index= 0; index< GetBlockNb(p); index++)
    { GetBlockGem(p,index)->y= cur_y;
      cur_y+= param->Get_Height_Gem_In_Pixel();
    }
    p= GetNextBlock(p);
  }      
  */
  unsigned multiplier;
  multiplier= (unsigned) (Display::GetTimeSlice(UPDATE_QUANTUM));
  
  if (multiplier== 0) return; // return KD_AR_UPDATE_NOTHING;
 /* return_value= */UpdateBlocks (multiplier);
  p= GetFirstBlock();
  
  while (GetBlockNb(p))
  { ind_off_y= GetBlockExtra(p);
    if (ind_off_y== 0) off_y= 0;
    else { off_y= Anim_OffsetY[ind_off_y--];
           SetBlockExtra (p,ind_off_y);
         }
         
    cur_y= GetBlockPosY(p)+ off_y;
    for (index= 0; index< GetBlockNb(p); index++)
    { GetBlockGem(p,index)->y= cur_y;
      cur_y+= param->Get_Height_Gem_In_Pixel();
    }
    p= GetNextBlock(p);
  }

//  return return_value;
  return;
}


void KD_AnimatedRow::UpdateBlocks (unsigned multiplier)
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
    speed+= accel* multiplier;
    posy+= speed* multiplier;

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
          param->SetCheckOverflow();
          
          if (state== KD_BS_DROP)
          /* remember the first gem of the block has clashed */
          /* set_memo will be filled in this function */
             GetBlockGem(p,0)->SetNeedClashTest();
          else
          if (last_block!= NULL &&
/*              CanClash(GetBlockGem(p,0)->GetType(), 
                       GetBlockGem(last_block, GetBlockNb(last_block)-1)->GetType()) )*/
              GetBlockGem(p,0)->CanClashWith(
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

            /* start the spring animation if the gem have been dropped */  
            if (state!= KD_BS_UP)
              SetBlockExtra (last_block, ANIM_OFF_SIZE);
            
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
      
      #ifdef DEBUG
      short res= 
      #endif 
      hand->TakeGems (GetBlockGems(p), GetBlockNb(p));
      #ifdef DEBUG
      assert (!res); /* should not fail */
      #endif

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


void KD_AnimatedRow::Display()
{ short* p_block= GetFirstBlock();
  short  nb= GetBlockNb(p_block);
    
  while (nb!= 0)
  { KD_Gem* gem= GetBlockGem(p_block, nb- 1);
    assert (gem);
    
#ifdef DEBUG    
      if (!(param->IsRemoving()))
        if (gem->IsRemoving()) { PrintRow(); assert (0); }
#endif       
      
    gem->Display(gem->x, gem->y);
     
    nb--;
    if (nb== 0)
    { if (IsLastBlock(p_block)) return;
      p_block= GetNextBlock(p_block);
      nb= GetBlockNb(p_block);
      assert (nb);
    }
  }
}
