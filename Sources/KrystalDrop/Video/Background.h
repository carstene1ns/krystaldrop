#ifndef BACKGROUND_H
#define BACKGROUND_H

#define KD_BACKGROUND_SPR 20

class KD_DisplayableResource;
class KD_DisplayableInstance;

class KD_Background
{ protected:
    void  InitBackground(KD_DisplayableResource *spr); 
   float X_S[KD_BACKGROUND_SPR];
   float Y_S[KD_BACKGROUND_SPR];
  //static KD_Background* p_background;
  
   KD_DisplayableResource* spr;
   KD_DisplayableInstance* backgr;
  
  public:
   KD_Background(KD_DisplayableResource *spr);
  ~KD_Background();
  
   void  Display();  

  //static KD_Background* GetBackground();
};

#endif
