#include "../../global.h"

#include <string.h>
#include <SDL/SDL.h>
#include "AnimTextEvent.h"
#include "../../../KDpp/Video/Font.h"

/* BouncingText */
KD_BouncingText::KD_BouncingText (char* Text, KD_Font* Font, int CenterX, int CenterY)
{ assert (Text);
  assert (*Text);
  text= strdup (Text);
  assert (text);
  
  len= strlen (text);
  X_L= (float*) malloc(len* sizeof(float));
  Y_L= (float*) malloc(len* sizeof(float));
  A_L= (float*) malloc(len* sizeof(float));  
  assert (X_L);
  assert (Y_L);
  assert (A_L);
  
  assert (Font);  
  font= Font;
  if (font!= NULL) space= font->ComputeLength (" ")*8/7;
  
  C_X= CenterX;
  C_Y= CenterY;

  for (unsigned i= 0; i< len; i++)
  { X_L[i]= C_X+ (i- len/2)* space;    
    Y_L[i]= C_Y; /* not really Y-centered actually */    
    A_L[i]= 0;
  }
  
  nb= 0;
  IsRemoving= 0;
}


KD_BouncingText::~KD_BouncingText()
{ font= NULL;  

  FREE (X_L);
  FREE (Y_L);
  FREE (A_L);
  FREE (text);
  len= 0;
}


#define famp(a) ((a)>10?15-(a):(a)-5)
#define fp(a) ((a)>0?a:0)
void KD_BouncingText::Update (float timeElapsed)
{ for (unsigned i= 0; i< len; i++)
  { float time_r= timeElapsed* 8.0f + i* 12;
    float time_s= timeElapsed* 4.0f+ i*8;
    float time_v= time_s- floor(time_s/20)* 20;
    float time_p= fp(famp(time_v));
    A_L[i]= fp(0.2- famp(time_v))* cos(time_r)/12;
    Y_L[i]= C_Y- fabs (time_p*space/4*cos(time_r) );
  } 
  
  if (IsRemoving)
  { nb+= timeElapsed* len/ 2000;
    if (nb> len) 
    { nb= len;
      AutoDestructEvent();
    }
  }
}


void KD_BouncingText::Display()
{ assert (font);
  unsigned n= (int) nb;
  
  for (unsigned i= 0; i< len; i++)
    if (IsRemoving== 0 || (i> n && i< (len- n)) )
#ifndef NO_OPENGL 
    font->xycoloralpharotozoomcenteredprintf (255, 255, 255, 255, 1, 1,
               X_L[i], Y_L[i], A_L[i], X_L[i], Y_L[i], "%c", text[i]);
#else
    font->xycenteredprintf (X_L[i], Y_L[i], "%c", text[i]);  
#endif
}


void KD_BouncingText::RemoveText()
{ IsRemoving= 1;
}



/* MessageText */
KD_MessageText::KD_MessageText (char* Text, KD_Font* Font, int CenterX, int CenterY)
{ assert (Text);
  assert (*Text);
  text= strdup (Text);
  assert (text);
  
  len= strlen (text);
  X_L= (float*) malloc(len* sizeof(float));
  Y_L= (float*) malloc(len* sizeof(float));
  A_L= (float*) malloc(len* sizeof(float));  
  assert (X_L);
  assert (Y_L);
  assert (A_L);  
  
  assert (Font);  
  font= Font;
  if (font!= NULL) space= font->ComputeLength (" ")*8/7;
  
  C_X= CenterX;
  C_Y= CenterY;

  for (unsigned i= 0; i< len; i++)
  { X_L[i]= C_X+ (i- len/2)* space;    
    Y_L[i]= C_Y; /* not really Y-centered actually */    
    A_L[i]= 0;    
  }

  C_X= CenterX;
  C_Y= CenterY;
  IsRemoving= 0;
}


KD_MessageText::~KD_MessageText()
{ font= NULL;  

  FREE (X_L);
  FREE (Y_L);
  FREE (A_L);
  FREE (text);
  len= 0;
}


void KD_MessageText::Update (float timeElapsed)
{ if (IsRemoving)
  { short end= 1;
    float time_l= timeElapsed* 1;
    
    for (unsigned i= 0; i< len; i++)
    { if (X_L[i]< SCR_HW) X_L[i]-= time_l; else X_L[i]+= time_l;
      if (X_L[i]> -space && X_L[i]< SCR_W+ space) end= 0;
    }
    if (end== 1) 
      AutoDestructEvent();
  }
  else
  { for (unsigned i= 0; i< len; i++)
  { float time_l= timeElapsed* 450 - i* 30;
    if (time_l> 800)
      X_L[i]= C_X+ (i*1.0f- len/2.0f)* space;
    else 
    { X_L[i]= C_X+ cos(time_l/400)* (800- time_l)+ (i*1.0f- len/2.0f)* space;
      Y_L[i]= C_Y+ sin(time_l/400)* (800- time_l);
    }
  }
  }
}


void KD_MessageText::Display()
{ assert (font);
  
  for (unsigned i= 0; i< len; i++)
    font->xycenteredprintf (X_L[i], Y_L[i], "%c", text[i]);
}


void KD_MessageText::RemoveText()
{ IsRemoving= 1;
}
