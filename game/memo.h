#ifndef MEMO_H
#define MEMO_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <deque>
using namespace std;

class KD_Gem;

/*typedef struct { short   x;// useless;
                 KD_Gem* p_gem; } KD_GemLocation;*/

class KD_Memo
{ private:
 //  deque <KD_GemLocation> memo; /* memorize which gems to look for possible clash */
  deque <KD_Gem*> memo;

  public:
     
   short   GetOffset (short index);
   KD_Gem* GetGem    (short index);
   short   GetSize();
   void    Remember (/*short X,*/ KD_Gem* p_Gem);  
   void    Forget   (short index);
   void    Forget   (KD_Gem* p_Gem);
};

#endif