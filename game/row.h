/* row.h

   One vertical row of gems
*/   

#ifndef ROW_H
#define ROW_H

#define KD_E_ROWFULL          -20
#define KD_E_BUFOVERFLOW      -21
#define KD_E_GEMINDEXINVALID  -22
#define KD_E_ADDIMPOSSIBLENOW -23

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
   KD_Set* set;
   
   /* gems speed parameters */
   KD_Parameters* param;
   
   
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
    KD_Gem* GetFirstGem();
    KD_Gem* GetNextGem();
    /* the order returned is not from top to bottom or from bottom to top. It involves blocks. */
    
#ifdef DEBUG
    void PrintRow();
#endif

};

#endif