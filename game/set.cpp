#include <assert.h>
#include <stdlib.h>

#include "set.h"

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
  for (i= 0; i< Width; i++)
  { field[i]= new KD_Row(Height, hand, param);
	  assert (field[i]);
  }
}

KD_Set::~KD_Set ()
{	if (field!= NULL)
	{ int i;
		for (i= 0; i< height; i++)
		if (field[i]!= NULL) delete field[i];
    free (field);
		field= NULL;
	}
  
	if (hand!= NULL)
	{	delete hand;
		hand= NULL;
	}
}