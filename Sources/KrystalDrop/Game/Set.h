#ifndef SET_H
#define SET_H

/*#define KD_E_CANTMOVE      -31*/
#define KD_E_ADDIMPOSSIBLE -32

class KD_AnimatedRow;
class KD_Gem;
class KD_Row;
class KD_Hand;
class KD_Memo;
class KD_MemoBonus;
class KD_BurstGems;
class KD_Parameters;
         
class KD_GenericSet
{ protected:public:
   KD_Hand*         hand;
   KD_Parameters*   param;
   KD_AnimatedRow** field;
   KD_Memo*         memo; /* which gems we should check for a burst */
   short height;
   short width;

  public:
           KD_GenericSet (int Width, int Height, 
                          int max_in_hand, KD_Parameters* Param);
  virtual ~KD_GenericSet();
  
#ifdef DEBUG_SANITY_CHECK
  void SanityCheck();
#endif
  
  signed IsUpFinished();
  signed IsLineDown();
    
KD_Parameters* GetParameters();
 KD_Memo* GetMemo();
 KD_Hand* GetHand();
    short GetMaxHeight();
  
   signed AddLineAtTop (KD_Gem** Gems);
    void  Update();
   signed TakeGems (short pos);
   signed DropGems (short pos);
   signed RemoveGems();
    void  MarkAsToBeRemoved (KD_Gem* Gem);      
    void  ResetVisitedFlag();

   
    void  Display(); /* draw on screen */
    
    /* Clash tests */
  virtual signed TestBurstStart ()= 0;
  virtual   void RecurseBurst   (short row, short gem, short type)= 0;

#ifdef DEBUG
short SearchGem (KD_Gem* gem);
#endif
};


class KD_Set: public KD_GenericSet
{ private: 
    short clash_type; /* ice blocks will be transformed into gems of 
                         type clash_type if a clash occurs */
  KD_BurstGems burst_gems();
  KD_MemoBonus memo_bonus();
  
  public:
           KD_Set (int Width, int Height, 
                   int max_in_hand, KD_Parameters* Param);
  //virtual ~KD_Set();
 
  virtual signed TestBurstStart ();
  virtual   void RecurseBurst   (short row, short gem, short type);
};

#endif
