#ifndef SET_H
#define SET_H

#include "hand.h"
#include "row.h"

class KD_Set
{ private:
   char* content;
   KD_Hand* hand;
   KD_Row** field;
   
   int height_pixel;
   int gem_height;
   int gem_width;
   int gem_speed_down;
   int gem_accel_hand;   
   
  public:
   KD_Set();
   KD_Set (int Width, int Height, int max_in_hand);
  ~KD_Set();
  
   signed AddLineAtTop (KD_Gem** Gems);
   signed TestClash();
   signed Update();
   signed TakeGems (KD_Gem* src, int count, int row);
   signed DropGems (KD_Gem* dest, int row);
   signed RemoveGem (KD_Gem* gem, int row, int index); /* check param */
   
   void SetParameters (int Height_Pixel, int Gem_Height, int Gem_Width, int Gem_Speed_Down, int Gem_Accel_Hand);
};

#endif