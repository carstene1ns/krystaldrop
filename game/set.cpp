#include <assert.h>
#include <stdlib.h>

#include "set.h"

#ifdef DEBUG
#include "stdio.h"
#endif

KD_Set::KD_Set (int Width, int Height, int max_in_hand, KD_Parameters* Param)
{ int i;
	
  field= (KD_Row**) malloc (Height* sizeof(KD_Row*));
  assert (field);

  assert (max_in_hand);
  hand= new KD_Hand(max_in_hand);
  assert (hand);

  assert (Param);
  param= Param;
	
  assert (Width> 0);
  width= Width;
  for (i= 0; i< width; i++)
  { field[i]= new KD_Row(Height, i* param->Get_Height_Gem_In_Pixel(), 
                         hand, param);
	assert (field[i]);
  }
  
  enum_row= 0;
  pos= 0;
}


KD_Set::~KD_Set ()
{ if (field!= NULL)
  { int i;
	for (i= 0; i< height; i++)
	if (field[i]!= NULL) delete field[i];
    free (field);
	field= NULL;
	}
  
  if (hand!= NULL)
  { delete hand;
	hand= NULL;
  }
}


signed KD_Set::MoveLeft()
{ if (pos== 0) return KD_E_CANTMOVE;
  pos--;
  
  return 0;
}


signed KD_Set::MoveRight()
{ if (pos== width- 1) return KD_E_CANTMOVE;
  pos++;
  
  return 0;
}


signed KD_Set::TakeGems()
{ assert (field[pos]);
  signed status;
  
  status= field[pos]->TakeFromBottom();
  /* check if all the gems have is_gem_down when a line is going down */
  /* if that's so, then the line going down has completely disappeared */
  /* this is quite improbable even width> 3, or the player is moving 
     extremely fast :-) */
  /* ## piece of code not tested */
  if (status== KD_S_LINEDOWNBROKEN)
  { signed index;
    
    for (index= 0; index< width; index++)
    { assert (field[index]);
      if (field[index]->is_gem_down== 1) break;
    }
    if (index== width)
    { /* the unlikely case has occurred. */
      param->ClearLineDown();
    }
    status= 0;
  }
  
  return status;
}


signed KD_Set::DropGems()
{ assert (field[pos]);
  
  return field[pos]->DropAtBottom();
}


signed KD_Set::AddLineAtTop (KD_Gem** Gem)
{ assert (Gem);
  assert (param);
  int index;
  int status;
  
  if (param->IsLineDown())
    return KD_E_ADDIMPOSSIBLE;
  
  for (index= 0; index< width; index++)
  { assert (Gem[index]);
    assert (field[index]);
    status= field[index]->AddAtTop (Gem[index]);
#ifdef DEBUG
    if (status== KD_E_ROWFULL) printf ("Row full has occured in KD_Set::AddLineAtTop()\n");
#endif      
    if (status!= 0 &&
        status!= KD_E_ROWFULL) /* this is not too serious, the player will lose soon. */
                               /* # maybe it could create a memory leak though. */
                               /* # one solution could be to write NULL in Gem
                                    when a gem has been successfully added. 
                                    When the function returns, the non-NULL 
                                    pointers have to be freed. */
    { /* let's hope that if there is an error, it's at the beginning of the line */
      /* well, for now, it is impossible to have an error other than KD_E_ROWFULL. */
      assert (index== 0);
      return status;
    }
  }
  
  return 0;
}


signed KD_Set::RemoveGem (KD_Gem* gem, int row)
{ assert (field[row]);
  
  return field[row]->RemoveGem (gem);
}


signed KD_Set::Update()
{ assert (field);
  signed index;
 // signed status;
  
  for (index= 0; index< width; index++)
  { assert (field[index]);
    /*status=*/ field[index]->Update();
    /* check clash bit */
    /* check line broken ended */
  }
  
  return 0;
}


KD_Gem* KD_Set::GetFirstGem()
{ enum_row= 0;
  return field[0]->GetFirstGem();
}


KD_Gem* KD_Set::GetNextGem()
{ KD_Gem* gem= field[0]->GetNextGem(); 
  
  if (gem== NULL)
  { if (++enum_row== width) return NULL;
    else return field[enum_row]->GetNextGem();
  }
  
  return gem;
}
