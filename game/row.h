/* row.h

   One vertical row of gems
*/   

#ifndef ROW_H
#define ROW_H

#define KD_E_ROWFULL          -20
#define KD_E_BUFOVERFLOW      -21
#define KD_E_GEMINDEXINVALID  -22
#define KD_E_ADDIMPOSSIBLENOW -23

#define KD_E_NOMOREGEM -32768 
/* need a large number because it is used in functions where Y (a signed integer) is returned.
   A vertical offset of -32768 is unlikely to occur. */

#include "hand.h"
#include "parameter.h"

// forward declaration
class KD_Set;

class KD_Row
{ private:
public:
   short* content;
   short* content_browse; /* used by GetFirstY and GetNextY */
   short  content_browse_rest; /* used by GetFirstY and GetNextY */
   short content_size;
   short height_in_gem;
   KD_Hand* hand;
   KD_Parameters* param;
   KD_Set* set;
   
   /* gems speed parameters */
   short speed_line_down;
   short accel_line_down;
   
   /* graphic parameter */
   short gem_height_in_pixel;
   
  protected:
  public:
   short CountGems();
   signed SplitLastBlockAt (short* last_block, short index);
      
  public:
    KD_Row();
    KD_Row (short Height_In_Gems, KD_Hand* Hand, KD_Parameters* Param);
   ~KD_Row();
   
    void SetSet   (KD_Set* Set); /* nice name */
    void SetParam (KD_Parameters* Param);
   
    /* moving gems */
    signed AddAtTop (KD_Gem* Gem);
    signed EmptyHandAtBottom ();
    signed FillHandFromBottom();
    signed Update();
    signed RemoveGem (KD_Gem* gem, int index);
    
    /* drawing on screen */
    signed GetFirstY();
    signed GetNextY();
    /* the order returned is not from top to bottom or from bottom to top. It involves blocks. */
    
#ifdef DEBUG
    void PrintRow();
#endif

};

#endif